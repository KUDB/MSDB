#pragma once
#ifndef _MSDB_OP_SE_HUFFMAN_DECODE_ACTION_H_
#define _MSDB_OP_SE_HUFFMAN_DECODE_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <system/storageMgr.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/wtChunk.h>
#include <compression/seHuffmanChunk.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
class se_huffman_decode_action : public opAction
{
public:
	se_huffman_decode_action();
	virtual ~se_huffman_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	pSeHuffmanChunk makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc,
								chunkId cid, coor chunkCoor);

	template <typename Ty_>
	void decompressAttribute(std::shared_ptr<wavelet_encode_array>outArr, pAttributeDesc attrDesc, pQuery qry)
	{
		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//

		auto outArrId = outArr->getId();
		auto attrId = attrDesc->id_;
		auto arrIndex = arrayMgr::instance()->getAttributeIndex(outArrId, attrId);
		if (arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<position_t, Ty_>>(arrIndex);
		auto cit = outArr->getChunkIterator(iterateMode::ALL);
		std::vector<uint64_t> offsets = this->getSeqOffInBand<Ty_>(outArr);

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//

		size_t currentThreadId = 0;
		this->threadCreate(_MSDB_ACTION_THREAD_NUM_);

		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				// make chunk
				chunkId cid = cit->seqPos();
				coor chunkCoor = cit->coor();
				auto inChunk = this->makeInChunk(outArr, attrDesc, cid, chunkCoor);
				inChunk->setTileOffset(offsets);
				auto outChunk = outArr->makeChunk(*inChunk->getDesc());

				io_service_->post(boost::bind(&se_huffman_decode_action::decompressChunk<Ty_>, this,
												std::static_pointer_cast<wtChunk>(outChunk), inChunk, qry, outArr, attrId, mmtIndex, currentThreadId));
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

	template <typename Ty_>
	std::vector<uint64_t> getSeqOffInBand(std::shared_ptr<wavelet_encode_array> outArr)
	{
		dimension blockDims = outArr->getDesc()->getDimDescs()->getChunkDims();
		size_t blockCapa = blockDims.area();
		dimension bandDims = blockDims / std::pow(2, outArr->getMaxLevel() + 1);
		size_t bandCapa = bandDims.area();

		std::vector<uint64_t> offsets;
		auto iit = itemRangeItr(nullptr, eleType::CHAR, blockDims, bandDims);		// type not necessary

		while (iit.isEnd() != true)
		{
			offsets.push_back(iit.seqPos() * sizeof(Ty_));
			++iit;
		}

		return offsets;
	}

	template <typename Ty_>
	void decompressChunk(pWtChunk outChunk, pSeChunk inChunk, pQuery qry, std::shared_ptr<wavelet_encode_array> outArr, attributeId attrId,
						 std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex, const size_t parentThreadId)
	{
		auto threadId = getThreadId();

		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::COMPUTING);
		//----------------------------------------//
		auto maxLevel = outArr->getMaxLevel();
		arrayId arrId = outArr->getId();

		bool hasNegative = false;
		if ((Ty_)-1 < 0)
		{
			hasNegative = true;
		}

		// TODO::Create se_compression_array, seChunk
		// Make seChunk in se_compression_array
		this->requiredBitsFindingForChunk<Ty_>(inChunk, mmtIndex, maxLevel, hasNegative);

		//----------------------------------------//
		qry->getTimer()->nextWork(threadId, workType::IO);
		//----------------------------------------//

		pSerializable serialChunk
			= std::static_pointer_cast<serializable>(inChunk);
		storageMgr::instance()->loadChunk(arrId, attrId, inChunk->getId(),
										  serialChunk);

		//----------------------------------------//
		qry->getTimer()->nextWork(threadId, workType::COMPUTING);
		//----------------------------------------//

		outChunk->setLevel(inChunk->getLevel());
		outChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
		outChunk->makeBlocks();
		outChunk->bufferCopy(inChunk);
		outChunk->setSerializedSize(inChunk->getSerializedSize());

		//----------------------------------------//
		qry->getTimer()->pause(threadId);
		//========================================//
	}

	template <typename Ty_>
	void requiredBitsFindingForChunk(pSeChunk inChunk,
									 std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
									 size_t maxLevel,
									 bool hasNegative)
	{
		auto dSize = inChunk->getChunkCoor().size();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;

		// For Level 0
		this->findRequiredBitsForRootLevel<Ty_>(inChunk,
												mmtIndex,
												numBandsInLevel,
												hasNegative);

		// For child level
		this->findRequiredBitsForChildLevel<Ty_>(inChunk,
												 mmtIndex,
												 maxLevel,
												 numBandsInLevel,
												 hasNegative);
	}

	template <typename Ty_>
	void findRequiredBitsForRootLevel(pSeChunk inChunk,
									  std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
									  const size_t numBandsInLevel,
									  const bool hasNegative)
	{
		auto chunkCoor = inChunk->getChunkCoor();
		auto blockLevel = mmtIndex->getBlockLevel();
		auto mNode = mmtIndex->getNode(chunkCoor, blockLevel);
		bit_cnt_type fromMMT = getRBitFromMMT<Ty_>(mNode, hasNegative);

		// TODO::Synopsis Delta Decoding
		//inChunk->setMin(mNode->getMin<Ty_>());
		inChunk->setMin(0);

		for (size_t band = 0; band <= numBandsInLevel; ++band)
		{
			inChunk->rBitFromMMT.push_back(fromMMT);
		}
	}

	template <typename Ty_>
	void findRequiredBitsForChildLevel(pSeChunk inChunk,
									   std::shared_ptr<MinMaxTreeImpl<position_t, Ty_>> mmtIndex,
									   const size_t maxLevel,
									   const size_t numBandsInLevel,
									   const bool hasNegative)
	{
		auto dSize = inChunk->getChunkCoor().size();
		for (size_t level = 1; level <= maxLevel; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			coorItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + inChunk->getChunkCoor() * innerSpace);
				auto mNode = mmtIndex->getNode(innerCoor, mmtIndex->getBlockLevel() - level);
				bit_cnt_type rbFromMMT = std::max(static_cast<int64_t>(getRBitFromMMT<Ty_>(mNode, hasNegative) - (int64_t)level), static_cast<int64_t>(static_cast<char>(hasNegative)));
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					inChunk->rBitFromMMT.push_back(rbFromMMT);
				}

				++innerItr;
			}
		}
	}
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SE_HUFFMAN_DECODE_ACTION_H_