#include <pch.h>
#include <op/compass_decode/compass_decode_action.h>
#include <array/memBlockArray.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
compass_decode_action::compass_decode_action()
{
}

compass_decode_action::~compass_decode_action()
{
}

const char* compass_decode_action::name()
{
	return "compass_decode_action";
}

pArray compass_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	auto outArr = std::make_shared<memBlockArray>(this->getArrayDesc());
	outArr->copyChunkBitmap(this->getPlanInChunkBitmap());

	// Get Parameter - NumBin
	eleDefault numBins;
	std::static_pointer_cast<stableElement>(this->params_[1]->getParam())->getData(&numBins);

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		this->decodeAttribute(outArr, attr, numBins, qry);
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}

void compass_decode_action::decodeAttribute(pArray outArr, pAttributeDesc attrDesc, eleDefault numBins, pQuery qry)
{
	size_t currentThreadId = 0;

	//========================================//
	qry->getTimer()->nextWork(0, workType::PARALLEL);
	//----------------------------------------//

	this->threadCreate(_MSDB_ACTION_THREAD_NUM_);

	auto cit = outArr->getChunkIterator(iterateMode::EXIST);
	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cid = cit->seqPos();
			auto inChunk = this->makeInChunk(outArr, attrDesc, cid, numBins);
			auto outChunk = outArr->makeChunk(*inChunk->getDesc());

			io_service_->post(boost::bind(&compass_decode_action::decodeChunk, this,
							  outChunk, inChunk, qry, outArr, attrDesc->id_, currentThreadId));
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

void compass_decode_action::decodeChunk(pChunk outChunk, pCompassChunk inChunk, pQuery qry, pArray outArr, const attributeId attrId, const size_t parentThreadId)
{
	auto threadId = getThreadId();

	//========================================//
	qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
	//----------------------------------------//

	pSerializable serialChunk
		= std::static_pointer_cast<serializable>(inChunk);
	storageMgr::instance()->loadChunk(outArr->getId(), attrId, (inChunk)->getId(),
									  serialChunk);

	//----------------------------------------//
	qry->getTimer()->nextWork(threadId, workType::COMPUTING);
	//----------------------------------------//

	outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
	outChunk->makeBlocks();
	outChunk->bufferCopy(inChunk);
	outChunk->setSerializedSize(inChunk->getSerializedSize());

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}

pCompassChunk compass_decode_action::makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid, eleDefault numBins)
{
	auto inChunkDesc = std::make_shared<chunkDesc>(*inArr->getChunkDesc(attrDesc->id_, cid));
	auto inChunk = std::make_shared<compassChunk>(inChunkDesc);
	auto blockBitmap = this->getPlanBlockBitmap(cid);
	if (blockBitmap)
	{
		inChunk->copyBlockBitmap(blockBitmap);
	} else
	{
		// If there were no bitmap, set all blocks as true.
		inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
	}
	inChunk->setNumBins(numBins);
	inChunk->makeBlocks();

	return inChunk;
}
}		// core
}		// msdb