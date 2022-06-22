#pragma once
#ifndef _MSDB_OP_MMT_DELTA_ENCODE_ACTION_H_
#define _MSDB_OP_MMT_DELTA_ENCODE_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <array/flattenChunk.h>
#include <index/mmt.h>
#include <query/opAction.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/mmt_delta_encode/mmt_delta_encode_array.h>

namespace msdb
{
namespace core
{
class mmt_delta_encode_action : public opAction
{
public:
	mmt_delta_encode_action();
	~mmt_delta_encode_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

	template<class Ty_>
	void saveAttribute(std::shared_ptr<wavelet_encode_array> outArr, std::shared_ptr<wavelet_encode_array> inArr, pAttributeDesc attrDesc);

	template<class Ty_>
	void chunkEncode(pChunk oit, pChunk iit, 
					 const dimension& synopsisDim, 
					 const dimension& blockSpaceDim,
					 std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex);
};

template<class Ty_>
void mmt_delta_encode_action::saveAttribute(std::shared_ptr<wavelet_encode_array> outArr, std::shared_ptr<wavelet_encode_array> inArr, pAttributeDesc attrDesc)
{
	auto arrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getId(), attrDesc->id_);
	if (arrIndex->getType() != attrIndexType::MMT)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
	}
	auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<Ty_>>(arrIndex);
	auto cit = inArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
	size_t wtLevel = inArr->getMaxLevel();
	dimension chunkDim = inArr->getDesc()->getDimDescs()->getChunkDims();
	dimension synopsisDim = chunkDim / pow(2, wtLevel + 1);
	dimension blockSpaceDim = inArr->getOrigianlChunkDims().divCoor(chunkDim);

	while (!cit->isEnd())
	{
		// Make new chunk
		auto cDesc = (**cit)->getDesc();
		auto deltaChunk = std::static_pointer_cast<wtChunk<Ty_>>(outArr->makeChunk(*cDesc));
		deltaChunk->setLevel(wtLevel);
		// MMT only modify synopsis section, copy the rest values.
		deltaChunk->bufferCopy(**cit);
		this->chunkEncode(deltaChunk, **cit, synopsisDim, blockSpaceDim, mmtIndex);

		++(*cit);
	}

	//outArr->setMMT(attrDesc->id_, mmtIndex);
}

template<class Ty_>
void mmt_delta_encode_action::chunkEncode(pChunk outChunk, pChunk inChunk,
										  const dimension& synopsisDim, 
										  const dimension& blockSpaceDim,
										  std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex)
{
	//auto ibItr = inChunk->getBlockIterator();
	//auto obItr = outChunk->getBlockIterator();

	// wtChunk should have its flattenBlock, or the chunk should not be exist.0
	assert(inChunk->getBlockBitmap()->isExist(0) == true);
	auto inBlock = inChunk->getBlock(0);
	auto outBlock = outChunk->makeBlock(0);

	dimension originalChunkCoor = inChunk->getChunkCoor().divCoor(blockSpaceDim);
	dimension originalBlockCoor = inChunk->getChunkCoor() % blockSpaceDim;

	// Iterator synopsis item
	auto iit = inBlock->getItemRangeIterator(range(synopsisDim));
	auto oit = outBlock->getItemRangeIterator(range(synopsisDim));

	auto node = mmtIndex->getNode(originalChunkCoor, originalBlockCoor, mmtIndex->getBlockLevel());
	Ty_ nodeMin = node->getMin<Ty_>();

	while (!iit->isEnd())
	{
		auto inValue = (**iit).get<Ty_>();
		auto outValue = inValue - nodeMin;
		(**oit).set<Ty_>(outValue);
		++(*iit);
		++(*oit);
	}

//	while (!ibItr->isEnd())
//	{
//		if(ibItr->isExist())
//		{
//			outChunk->makeBlock(ibItr->seqPos());
//
//			auto iit = (**ibItr)->getItemIterator();
//			auto oit = (**obItr)->getItemIterator();
//			auto node = mmtIndex->getNode(inChunk->getDesc()->chunkCoor_, ibItr->coor(), mmtIndex->getBlockLevel());
//			Ty_ nodeMin = node->getMin<Ty_>();
//
//#ifndef NDEBUG
//			Ty_ min_ = (**iit).get<Ty_>();
//			Ty_ max_ = (**iit).get<Ty_>();
//#endif
//
//		// Block encode
//			while (!iit->isEnd())
//			{
//				auto inValue = (**iit).get<Ty_>();
//				auto outValue = inValue - nodeMin;
//				(**oit).set<Ty_>(outValue);
//#ifndef NDEBUG
//			//assert(outValue >= 0);
//			//if (min_ > outValue)
//			//	min_ = outValue;
//			//if (max_ < outValue)
//			//	max_ = outValue;
//#endif
//
//				++(*iit);
//				++(*oit);
//			}
//
//#ifndef NDEBUG
//		//BOOST_LOG_TRIVIAL(trace) << "Delta encode " << inChunk->getDesc()->chunkCoor_.toString() << "|" << ibItr->coor().toString() << "=> min: " << static_cast<int64_t>(min_) << ", max: " << static_cast<int64_t>(max_) << ", mmt: " << node->toString<Ty_>();
//#endif
//		}
//
//
//		++(*ibItr);
//		++(*obItr);
//	}
}
}		// core
}		// msdb
#endif		// _MSDB_OP_MMT_DELTA_ENCODE_ACTION_H_