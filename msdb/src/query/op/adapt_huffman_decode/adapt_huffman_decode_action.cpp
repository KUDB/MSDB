#include <pch.h>
#include <op/adapt_huffman_decode/adapt_huffman_decode_action.h>
#include <array/memBlockArray.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
adapt_huffman_decode_action::adapt_huffman_decode_action()
{
}
adapt_huffman_decode_action::~adapt_huffman_decode_action()
{
}
const char* adapt_huffman_decode_action::name()
{
	return "adapt_huffman_decode_action";
}
pArray adapt_huffman_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	pArray sourceArr = inputArrays[0];
	arrayId arrId = sourceArr->getId();

	auto outArr = std::make_shared<memBlockArray>(this->getArrayDesc());
	outArr->copyChunkBitmap(this->getPlanInChunkBitmap());

	for (auto attr : *sourceArr->getDesc()->attrDescs_)
	{
		this->loadAttribute(outArr, attr, qry);
	}

	//----------------------------------------//
	qry->getTimer()->pause(0);
	//========================================//

	return outArr;
}
void adapt_huffman_decode_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
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
			auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
			auto outChunk = outArr->makeChunk(*inChunk->getDesc());

			io_service_->post(boost::bind(&adapt_huffman_decode_action::loadChunk, this,
							  outArr, outChunk, inChunk, attrDesc->id_, qry, currentThreadId));
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
void adapt_huffman_decode_action::loadChunk(pArray outArr, pChunk outChunk, pAdaptHuffmanChunk inChunk, attributeId attrId, pQuery qry, const size_t paraentThreadId)
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

	outChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
	outChunk->makeBlocks();
	outChunk->bufferCopy(inChunk);
	outChunk->setSerializedSize(inChunk->getSerializedSize());

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}
pAdaptHuffmanChunk adapt_huffman_decode_action::makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid)
{
	auto inChunkDesc = std::make_shared<chunkDesc>(*inArr->getChunkDesc(attrDesc->id_, cid));
	auto inChunk = std::make_shared<adaptHuffmanChunk>(inChunkDesc);
	auto blockBitmap = this->getPlanBlockBitmap(cid);
	if (blockBitmap)
	{
		inChunk->copyBlockBitmap(blockBitmap);
	} else
	{
		// If there were no bitmap, set all blocks as true.
		inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
	}
	inChunk->makeBlocks();

	return inChunk;
}
}		// core
}		// msdb