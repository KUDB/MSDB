#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_
#define _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <array/blockChunk.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/mmt_delta_decode/mmt_delta_decode_array.h>
#include <op/mmt_delta_encode/mmt_delta_encode_array.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/wtChunk.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
class mmt_delta_decode_action : public opAction
{
public:
	mmt_delta_decode_action();
	~mmt_delta_decode_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

	template<class Ty_>
	void attributeDecode(std::shared_ptr<wavelet_encode_array> outArr,
						 std::shared_ptr<wavelet_encode_array> inArr,
						 pAttributeDesc attrDesc, pQuery qry)
	{
		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//
		// Get MMT from mmt_delta_encode_array
		auto attrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getId(), attrDesc->id_);
		if (attrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		//auto arrMMTIndex = std::static_pointer_cast<mmt>(attrIndex);
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<Ty_>>(attrIndex);
		auto cit = inArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		size_t wtLevel = inArr->getMaxLevel();
		dimension chunkDim = inArr->getDesc()->getDimDescs()->getChunkDims();
		dimension synopsisDim = chunkDim / pow(2, wtLevel + 1);
		dimension blockSpaceDim = inArr->getOrigianlChunkDims().divCoor(chunkDim);

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//
		size_t currentThreadId = 0;
		this->threadCreate();

		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				// Make new chunk
				//auto cDesc = (**cit)->getDesc();
				//pChunk outChunk = outArr->makeChunk(*cDesc);
				//outChunk->copyBlockBitmap((**cit)->getBlockBitmap());
				pWtChunk outChunk = std::static_pointer_cast<wtChunk>(outArr->makeChunk(attrDesc->id_, cit->seqPos()));
				outChunk->setLevel(wtLevel);
				outChunk->bufferCopy(**cit);

				io_service_->post(boost::bind(&mmt_delta_decode_action::chunkDecode<Ty_>, this,
								  outChunk, **cit, synopsisDim, blockSpaceDim, mmtIndex, qry, currentThreadId));
			}

			++(*cit);
		}

		this->threadStop();
		this->threadJoin();

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//========================================//
	}

	template<class Ty_>
	void chunkDecode(pChunk outChunk,
					 pChunk inChunk,
					 const dimension& synopsisDim,
					 const dimension& blockSpaceDim,
					 std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
					 pQuery qry,
					 const size_t parentThreadId)
	{
		auto threadId = getThreadId();
		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::COMPUTING);
		//----------------------------------------//

		// wtChunk should have its memBlock, or the chunk should not be exist.
		assert(inChunk->getBlockBitmap()->isExist(0) == true);
		auto inBlock = inChunk->getBlock(0);
		auto outBlock = outChunk->makeBlock(0);

		dimension originalChunkCoor = inChunk->getChunkCoor().divCoor(blockSpaceDim);
		dimension originalBlockCoor = inChunk->getChunkCoor() % blockSpaceDim;

		// Iterator synopsis item
		auto iit = inBlock->getItemRangeIterator(range(synopsisDim));
		auto oit = outBlock->getItemRangeIterator(range(synopsisDim));

		// Get mmtNode
		// MMT is built from the origianl array
		auto node = mmtIndex->getNode(originalChunkCoor, originalBlockCoor, mmtIndex->getBlockLevel());
		Ty_ nodeMin = node->getMin<uint32_t>();

		while (!iit->isEnd())
		{
			auto inValue = (**iit).get<Ty_>();
			auto outValue = inValue + nodeMin;
			(**oit).set<Ty_>(outValue);
			++(*iit);
			++(*oit);
		}

		//auto ibItr = inChunk->getBlockIterator();
		//auto obItr = outChunk->getBlockIterator();
		//while (!ibItr->isEnd())
		//{
		//	if (ibItr->isExist())
		//	{
		//		outChunk->makeBlock(ibItr->seqPos());

		//		auto iit = (**ibItr)->getItemIterator();
		//		auto oit = (**obItr)->getItemIterator();
		//		auto node = mmtIndex->getNode(inChunk->getDesc()->chunkCoor_, ibItr->coor(), mmtIndex->getBlockLevel());

		//		// Block encode
		//		while (!iit->isEnd())
		//		{
		//			auto inValue = (**iit).get<Ty_>();
		//			auto outValue = inValue + node->getMin<Ty_>();
		//			(**oit).set<Ty_>(outValue);
		//			++(*iit);
		//			++(*oit);
		//		}
		//	}

		//	++(*ibItr);
		//	++(*obItr);
		//}

		//----------------------------------------//
		qry->getTimer()->pause(threadId);
		//========================================//
	}
};
}		// core
}		// msdb
#endif		// _MSDB_OP_MMT_DELTA_DECODE_ACTION_H_