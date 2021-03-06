#include <pch.h>
#include <op/lzw_huffman_encode/lzw_huffman_decode_action.h>
#include <array/flattenArray.h>
#include <system/storageMgr.h>
#include <util/threadUtil.h>
#include "lzwHuffmanArray.h"

namespace msdb
{
namespace core
{
lzw_huffman_decode_action::lzw_huffman_decode_action()
{
}
lzw_huffman_decode_action::~lzw_huffman_decode_action()
{
}
const char* lzw_huffman_decode_action::name()
{
	return "lzw_huffman_decode_action";
}
pArray lzw_huffman_decode_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	assert(inputArrays.size() == 1);

	//========================================//
	qry->getTimer()->nextJob(0, this->name(), workType::COMPUTING);
	//----------------------------------------//

	pArray sourceArr = inputArrays[0];

	pArrayDesc outArrDesc = std::make_shared<arrayDesc>(*sourceArr->getDesc());
	pArray outArr = std::make_shared<lzwHuffmanArray>(outArrDesc);
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
void lzw_huffman_decode_action::loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry)
{
	size_t currentThreadId = 0;

	//========================================//
	qry->getTimer()->nextWork(0, workType::PARALLEL);
	//----------------------------------------//

	this->threadCreate();

	auto cit = outArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
	while (!cit->isEnd())
	{
		if (cit->isExist())
		{
			chunkId cid = cit->seqPos();
			//auto inChunk = this->makeInChunk(outArr, attrDesc, cid);
			auto outChunk = outArr->makeChunk(attrDesc->id_, cid);
			outChunk->makeAllBlocks();

			io_service_->post(boost::bind(&lzw_huffman_decode_action::loadChunk, this,
										  qry, currentThreadId, outArr, attrDesc->id_, outChunk));
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
void lzw_huffman_decode_action::loadChunk(pQuery qry, const size_t paraentThreadId, pArray outArr, attributeId attrId, pChunk outChunk)
{
	auto threadId = getThreadId();

	//========================================//
	qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
	//----------------------------------------//

	pSerializable serialChunk
		= std::static_pointer_cast<serializable>(outChunk);
	storageMgr::instance()->loadChunk(outArr->getId(), attrId, outChunk->getId(),
									  serialChunk);

	//----------------------------------------//
	qry->getTimer()->pause(threadId);
	//========================================//
}
//pLzwHuffmanChunk lzw_huffman_decode_action::makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid)
//{
//	auto inChunkDesc = std::make_shared<chunkDesc>(*inArr->getChunkDesc(attrDesc->id_, cid));
//	auto inChunk = std::make_shared<lzwHuffmanChunk>(inChunkDesc);
//	auto blockBitmap = this->getPlanBlockBitmap(cid);
//	if (blockBitmap)
//	{
//		inChunk->copyBlockBitmap(blockBitmap);
//	}
//	else
//	{
//		// If there were no bitmap, set all blocks as true.
//		inChunk->replaceBlockBitmap(std::make_shared<bitmap>(inChunk->getBlockCapacity(), true));
//	}
//	inChunk->makeBlocks();
//
//	return inChunk;
//}
}		// core
}		// msdb