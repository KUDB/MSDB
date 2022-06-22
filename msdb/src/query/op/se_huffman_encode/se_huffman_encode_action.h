#pragma once
#ifndef _MSDB_OP_SE_HUFFMAN_ENCODE_ACTION_H_
#define _MSDB_OP_SE_HUFFMAN_ENCODE_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <system/storageMgr.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/se_huffman_encode/seHuffmanChunk.h>

namespace msdb
{
namespace core
{
class se_huffman_encode_action : public opAction
{
public:
	se_huffman_encode_action();
	virtual ~se_huffman_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	//pSeHuffmanChunk makeOutChunk(pWtChunk inChunk);

	//void compressAttribute(std::shared_ptr<wavelet_encode_array>inArr, pAttributeDesc attrDesc)
	template<typename Ty_>
	void compressAttribute(const concreteTy<Ty_>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc)
	{
		size_t mSizeTotal = 0;
		auto arrId = inArr->getId();
		auto cit = inArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		bool hasNegative = false;

		auto arrIndex = arrayMgr::instance()->getAttributeIndex(inArr->getId(), attrDesc->id_);
		if (arrIndex->getType() != attrIndexType::MMT)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_ATTR_INDEX_TYPE_DIFF));
		}
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<Ty_>>(arrIndex);
		dimension chunkDim(inArr->getDesc()->getDimDescs()->getChunkDims());

		if ((Ty_)-1 < 0)
		{
			hasNegative = true;
		}

		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto inChunk = std::static_pointer_cast<wtChunk<Ty_>>(**cit);
				auto cDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
				auto outChunk = std::static_pointer_cast<seHuffmanChunk<Ty_>>(outArr->makeChunk(cDesc));

				outChunk->setLevel(inChunk->getLevel());
				//outChunk->setSourceChunkId(inChunk->getSourceChunkId());
				outChunk->bufferRef(inChunk);
				outChunk->makeAllBlocks();

				this->compressChunk<Ty_>(outChunk, inChunk, mmtIndex, chunkDim, hasNegative);

				auto attr = outChunk->getDesc()->attrDesc_;
				storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
												  std::static_pointer_cast<serializable>(outChunk));

				mSizeTotal += outChunk->getSerializedSize();
			}

			++(*cit);
		}

		BOOST_LOG_TRIVIAL(info) << "Total Save Chunk: " << mSizeTotal << " Bytes";
	}

	template<>
	void compressAttribute(const concreteTy<float>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "se_huffman_compress not support data compression for float"));
	}
	template<>
	void compressAttribute(const concreteTy<double>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "se_huffman_compress not support data compression for float"));
	}

	template<typename Ty_>
	void compressChunk(std::shared_ptr<seHuffmanChunk<Ty_>> outChunk,
					   std::shared_ptr<wtChunk<Ty_>> inChunk,
					   std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
					   dimension& sourceChunkDim, bool hasNegative)
	{
		size_t dSize = inChunk->getDSize();

		pBlock outBlock = outChunk->getBlock(0);
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = inChunk->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, inChunk->getLevel() + 1);

		// For Level 0
		this->findRequiredBitsForRootLevel<Ty_>(outChunk, outBlock,
												mmtIndex,
												bandDims,
												numBandsInLevel, hasNegative);

		// For child level
		this->findRequiredBitsForChildLevel<Ty_>(outChunk, outBlock,
												 mmtIndex,
												 bandDims, inChunk->getLevel(),
												 numBandsInLevel, hasNegative);
	}

	//template <class Ty_>
	//bit_cnt_type findRequiredBits(pBlock curBlock, const range& bandRange)
	//{
	//	auto bItemItr = curBlock->getItemRangeIterator(bandRange);
	//	bit_cnt_type maxValueBits = 0;

	//	while (!bItemItr->isEnd())
	//	{
	//		Ty_ value = (**bItemItr).get<Ty_>();
	//		bit_cnt_type valueBits = static_cast<bit_cnt_type>(msb<Ty_>(abs_(value)));
	//		if (maxValueBits < valueBits)
	//		{
	//			maxValueBits = valueBits;

	//		}
	//		++(*bItemItr);
	//	}

	//	return maxValueBits;
	//}
	template <class Ty_>
	bit_cnt_type findRequiredBits(pBlock curBlock, const range& bandRange)
	{
		auto bItemItr = curBlock->getItemRangeIterator(bandRange);
		bit_cnt_type maxValueBits = 0;

		Ty_ signMask = ((Ty_)1) << (sizeof(Ty_) * CHAR_BIT - 1);
		bool hasNegative = false;		// to treat -128, 128

	#ifndef NDEBUG
		Ty_ maxValue = 0;
	#endif

		while (!bItemItr->isEnd())
		{
			Ty_ value = (**bItemItr).get<Ty_>();

			if (value & signMask)
			{
				value = ((Ty_)~value) + 1;
				hasNegative = true;
			}
			bit_cnt_type valueBits = static_cast<bit_cnt_type>(msb<Ty_>(abs_(value)));
			if (maxValueBits < valueBits)
			{
				maxValueBits = valueBits;
			#ifndef NDEBUG
				maxValue = value;
			#endif
			}
			++(*bItemItr);
		}

		if (!hasNegative && maxValueBits == 0)
		{
			return 0;				// no -128 and has only zeros
		}
		return maxValueBits + 1;	// In normal case, add sign bit
	}

	template <class Ty_>
	void findRequiredBitsForRootLevel(std::shared_ptr<seHuffmanChunk<Ty_>> outChunk, pBlock outBlock,
									  std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
									  const dimension& bandDims, const size_t numBandsInLevel,
									  const bool hasNegative)
	{
		auto chunkCoor = outChunk->getChunkCoor();
		auto blockLevel = mmtIndex->getBlockLevel();
		auto mNode = mmtIndex->getNode(chunkCoor, blockLevel);
		bit_cnt_type rbFromMMT = getRBitFromMMT<Ty_>(mNode, hasNegative);

		outChunk->setMin(0);

		if (rbFromMMT == 0)
		{
			for (size_t band = 0; band <= numBandsInLevel; ++band)
			{
				outChunk->rBitFromDelta.push_back(0);
				outChunk->rBitFromMMT.push_back(0);
			}
		}
		else
		{
			for (size_t band = 0; band <= numBandsInLevel; ++band)
			{
				//bit_cnt_type requiredBits = this->findRequiredBits<Ty_>(outBlock, getBandRange(band, bandDims)) + static_cast<char>(hasNegative);
				bit_cnt_type rbFromDelta = this->findRequiredBits<Ty_>(outBlock, getBandRange(band, bandDims));

				outChunk->rBitFromDelta.push_back(rbFromDelta);
				outChunk->rBitFromMMT.push_back(rbFromMMT);
			}
		}
	}

	template <class Ty_>
	void findRequiredBitsForChildLevel(std::shared_ptr<seHuffmanChunk<Ty_>> outChunk, pBlock outBlock,
									   std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
									   const dimension& bandDims,
									   const size_t maxLevel,
									   const size_t numBandsInLevel,
									   const bool hasNegative)
	{
		auto dSize = bandDims.size();
		for (size_t level = 1; level <= maxLevel; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			mdItr innerItr(innerSpace);
			auto blockLevel = mmtIndex->getBlockLevel();
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + outChunk->getChunkCoor() * innerSpace);
				auto mNode = mmtIndex->getNode(innerCoor, blockLevel - level);
				bit_cnt_type rbFromMMT = getRBitFromMMT<Ty_>(mNode, hasNegative);
				
				if (rbFromMMT == 0)
				{
					for (size_t band = 1; band <= numBandsInLevel; ++band)
					{
						outChunk->rBitFromDelta.push_back(0);
						outChunk->rBitFromMMT.push_back(0);
					}
				}
				else
				{
					// Minimum rBit = 2:: sign=1bit, value>=1bit 
					rbFromMMT = std::max(static_cast<int64_t>(rbFromMMT - (int64_t)level), static_cast<int64_t>(2));
					for (size_t band = 1; band <= numBandsInLevel; ++band)
					{
						dimension targetSp = getBandRange(band, bandDims * pow(2, level)).getSp() + innerItr.coor() * bandDims;
						dimension targetEp = targetSp + bandDims;

						bit_cnt_type rbFromDelta = this->findRequiredBits<Ty_>(outBlock, range(targetSp, targetEp));
						//bit_cnt_type rbFromDelta = this->findRequiredBits<Ty_>(outBlock, range(targetSp, targetEp));
						//if (rbFromDelta > 0)
						//{
						//	rbFromDelta += static_cast<char>(hasNegative);
						//}
						outChunk->rBitFromDelta.push_back(rbFromDelta);
						outChunk->rBitFromMMT.push_back(rbFromMMT);
					}
				}

				++innerItr;
			}
		}
	}
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SE_HUFFMAN_ENCODE_ACTION_H_