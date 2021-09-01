#include <pch.h>
#include <op/spiht_decode/spiht_decode_action.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <array/memChunk.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
spiht_decode_action::spiht_decode_action()
{
}

spiht_decode_action::~spiht_decode_action()
{
}

const char* spiht_decode_action::name()
{
	return "spiht_decode_action";
}

pArray spiht_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);
	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);

	pStableElement ele = std::static_pointer_cast<stableElement>(this->params_[1]->getParam());
	eleDefault maxLevel;
	ele->getData(&maxLevel);

	auto planBitmap = this->getPlanInChunkBitmap();
	auto arrDesc = this->getArrayDesc();
	dimension originalChunkDims = arrDesc->getDimDescs()->getChunkDims();
	for (dimensionId d = 0; d < arrDesc->getDSize(); ++d)
	{
		arrDesc->getDimDescs()->at(d)->chunkSize_ = arrDesc->getDimDescs()->at(d)->blockSize_;
	}

	auto outArr = std::make_shared<wavelet_encode_array>(this->getArrayDesc());
	outArr->setLevel(maxLevel);
	outArr->setOrigianlChunkDims(originalChunkDims);
	outArr->copyChunkBitmap(planBitmap);

	for (auto attr : *outArr->getDesc()->attrDescs_)
	{
		this->decodeAttribute(std::static_pointer_cast<wavelet_encode_array>(outArr), attr, qry);
	}
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

void spiht_decode_action::decodeAttribute(std::shared_ptr<wavelet_encode_array> outArr, 
										  pAttributeDesc attrDesc, pQuery qry)
{
	auto cit = outArr->getChunkIterator(iterateMode::ALL);
	auto attrId = attrDesc->id_;

	//========================================//
	qry->getTimer()->nextWork(0, workType::PARALLEL);
	//----------------------------------------//

	size_t currentThreadId = 0;
	this->threadCreate(_MSDB_ACTION_THREAD_NUM_);

	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cid = cit->seqPos();

			auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
			auto outChunk = outArr->makeChunk(*inChunk->getDesc());
			auto wtOutChunk = std::static_pointer_cast<wtChunk>(outChunk);
			wtOutChunk->setLevel(outArr->getMaxLevel());

			io_service_->post(boost::bind(&spiht_decode_action::decompressChunk, this,
							  wtOutChunk, inChunk, qry, outArr->getId(), attrId, currentThreadId));
		}

		++(*cit);
	}

	this->threadStop();
	this->threadJoin();

	//----------------------------------------//
	qry->getTimer()->nextWork(0, workType::COMPUTING);
	//========================================//

	this->getArrayStatus(outArr);
}

void spiht_decode_action::decompressChunk(pWtChunk outChunk, pSpihtChunk inChunk, pQuery qry, 
										  const arrayId arrId, const attributeId attrId, const size_t parentThreadId)
{
	auto threadId = getThreadId();

	//========================================//
	qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
	//----------------------------------------//
		
	pSerializable serialChunk
		= std::static_pointer_cast<serializable>(inChunk);

	storageMgr::instance()->loadChunk(arrId, attrId, inChunk->getId(), serialChunk);

	//----------------------------------------//
	qry->getTimer()->nextWork(threadId, workType::COMPUTING);
	//----------------------------------------//

	outChunk->setLevel(inChunk->getLevel());
	outChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
	outChunk->makeBlocks();
	outChunk->bufferCopy(inChunk);	// If ref bitmap from inChunk, it might be deleted in 'inChunk' when it destroied.
	outChunk->setSerializedSize(inChunk->getSerializedSize());

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}

pSpihtChunk spiht_decode_action::makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc, chunkId cid)
{
	auto inChunk = std::make_shared<spihtChunk>(arr->getChunkDesc(attrDesc->id_, cid));
	auto blockBitmap = this->getPlanBlockBitmap(cid);
	if (blockBitmap)
	{
		inChunk->copyBlockBitmap(blockBitmap);
	} else
	{
		// If there were no bitmap, set all blocks as true.
		inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
	}

	inChunk->setLevel(arr->getMaxLevel());
	inChunk->makeBlocks();

	return inChunk;
}
}		// core
}		// msdb