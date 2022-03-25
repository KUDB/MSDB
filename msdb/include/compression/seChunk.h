﻿#pragma once
#ifndef _MSDB_SE_CHUNK_H_
#define _MSDB_SE_CHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <array/block.h>
#include <compression/waveletUtil.h>
#include <numeric>

namespace msdb
{
namespace core
{
//class seChunk;
//using pSeChunk = std::shared_ptr<seChunk>;

template <typename Ty_>
class seChunk : public flattenChunk<Ty_>
{
public:
	seChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc), level_(0), rBitFromMMT(0), min_(0)
	{

	}
	virtual ~seChunk()
	{

	}

public:
	inline size_t getLevel()
	{
		return this->level_;
	}
	//chunkId getSourceChunkId();

	inline void setLevel(size_t level)
	{
		this->level_ = level;
	}
	//void setSourceChunkId(chunkId cid);

public:
	virtual void serialize(std::ostream& os) override
	{
		bstream bs;
		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->serialize<char>(bs);
			break;
		case eleType::INT8:
			this->serialize<int8_t>(bs);
			break;
		case eleType::INT16:
			this->serialize<int16_t>(bs);
			break;
		case eleType::INT32:
			this->serialize<int32_t>(bs);
			break;
		case eleType::INT64:
			this->serialize<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->serialize<int8_t>(bs);
			break;
		case eleType::UINT16:
			this->serialize<int16_t>(bs);
			break;
		case eleType::UINT32:
			this->serialize<int32_t>(bs);
			break;
		case eleType::UINT64:
			this->serialize<int64_t>(bs);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}

		this->serializedSize_ = bs.capacity();
		this->getOutHeader()->serialize(os);
		os.write(bs.data(), bs.capacity());
	}
	virtual void deserialize(std::istream& is) override
	{
		this->getHeader()->deserialize(is);
		this->updateFromHeader();

		bstream bs;
		bs.resize(this->serializedSize_);
		is.read(bs.data(), this->serializedSize_);

		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->deserialize<char>(bs);
			break;
		case eleType::INT8:
			this->deserialize<int8_t>(bs);
			break;
		case eleType::INT16:
			this->deserialize<int16_t>(bs);
			break;
		case eleType::INT32:
			this->deserialize<int32_t>(bs);
			break;
		case eleType::INT64:
			this->deserialize<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->deserialize<int8_t>(bs);
			break;
		case eleType::UINT16:
			this->deserialize<int16_t>(bs);
			break;
		case eleType::UINT32:
			this->deserialize<int32_t>(bs);
			break;
		case eleType::UINT64:
			this->deserialize<int64_t>(bs);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	void serializeGap(bstream& bs, int64_t gap)
	{
		if (gap < 0)
		{
			bs << setw(1) << 0x1;
			//BOOST_LOG_TRIVIAL(debug) << "GAP < 0";
		}
		else
		{
			bs << setw(1) << 0x0;
		}
		if (gap)
		{
			assert(abs_(gap) <= 128);
			bs << setw(abs_(gap));
			bs << (uint64_t)0;
		}
		bs << setw(1) << 0x1;
	}
	int64_t deserializeGap(bstream& bs)
	{
		bs >> setw(1);
		char negiveGap;
		bs >> negiveGap;

		size_t gap = 0;
		char gapBit = 0;
		do
		{
			assert(gap <= 128);
			bs >> gapBit;
			++gap;
		} while (gapBit != 1);

		if (negiveGap)
		{
			return (gap - 1) * -1;
		}
		return gap - 1;
	}

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		this->seEncode<Ty_>(bs);
	}

	template <typename Ty_>
	void seEncode(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);

		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);

#ifndef NDEBUG
		for (int d = 0; d < dSize; ++d)
		{
			assert(bandDims[d] > 0);	// Level is too high for block dim
		}
#endif

		size_t seqId = 0;
		// Level 0
		{
#ifndef NDEBUG
			//auto before = bs.capacity();
			//BOOST_LOG_TRIVIAL(debug) << "MIN: " << this->min_;
#endif

			for (size_t band = 0; band <= numBandsInLevel; ++band, ++seqId)
			{
				this->serializeBand<Ty_>(bs, myBlock, seqId, band, bandDims);
			}

#ifndef NDEBUG
			//auto synopsisSize = bs.capacity() - before;
			//BOOST_LOG_TRIVIAL(debug) << "Save Synopsis[" << this->desc_->id_ << "] : " << synopsisSize << " Bytes";
#endif
		}

		this->serializeChildLevelBand<Ty_>(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	template<typename Ty_>
	void serializeBand(bstream& bs, pBlock myBlock,
					   size_t seqId, size_t bandId, dimension& bandDims)
	{
		bit_cnt_type rbFromDelta = this->rBitFromDelta[seqId];
		bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];
#ifndef NDEBUG
		//BOOST_LOG_TRIVIAL(trace) << "seqId: " << static_cast<int>(seqId) << ", setw: " << static_cast<int>(rbFromDelta);
		//BOOST_LOG_TRIVIAL(trace) << "rBitFromMMT: " << static_cast<int>(rbFromMMT) << "/ from Delta: " << static_cast<int>(rbFromDelta);
		//if(rbFromMMT < rbFromDelta)
		//{
		//	BOOST_LOG_TRIVIAL(warning) << "rBitFromMMT: " << static_cast<int>(rbFromMMT) << "/ from Delta: " << static_cast<int>(rbFromDelta);
		//}
		//assert(rbFromMMT >= rbFromDelta);
#endif

		this->serializeGap(bs, rbFromMMT - rbFromDelta);
		bs << setw(rbFromDelta);
		Ty_ signMask = 0x1 << rbFromDelta - 1;

		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		while (!bItemItr->isEnd())
		{
			//Ty_ value = (**bItemItr).get<Ty_>();
			//bs << (**bItemItr).get<Ty_>();
			//++(*bItemItr);

			Ty_ value = (**bItemItr).get<Ty_>();
			//value -= this->min_;

			if (value < 0)
			{
				value = abs_(value);
				value |= signMask;
			}

			bs << value;
			++(*bItemItr);
		}
	}

	template <typename Ty_>
	void serializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		auto dSize = this->getDSize();

#ifndef NDEBUG
		std::vector<int64_t> gaps;
#endif

		for (size_t level = 1; level <= this->level_; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			mdItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + this->getChunkCoor() * innerSpace);
				//BOOST_LOG_TRIVIAL(trace) << "area: " << static_cast<int>(innerCoor.area());
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					dimension targetSp = getBandRange(band, bandDims * pow(2, level)).getSp() + innerItr.coor() * bandDims;
					dimension targetEp = targetSp + bandDims;
					auto bItemItr = inBlock->getItemRangeIterator(range(targetSp, targetEp));

					bit_cnt_type rbFromDelta = this->rBitFromDelta[seqId];
					bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];

#ifndef NDEBUG
					//BOOST_LOG_TRIVIAL(trace) << "seqId: " << static_cast<int>(seqId) << ", setw: " << static_cast<int>(rbFromDelta);
					//BOOST_LOG_TRIVIAL(trace) << "rBitFromMMT: " << static_cast<int>(rbFromMMT) << "/ from Delta: " << static_cast<int>(rbFromDelta) << "/ gap: " << static_cast<int>(rbFromMMT - rbFromDelta) << " (" << inBlock->getId() << ", " << band << ")";
					//BOOST_LOG_TRIVIAL(trace) << targetSp.toString() << ", " << targetEp.toString();
					//if (rbFromMMT < rbFromDelta)
					//{
					//	BOOST_LOG_TRIVIAL(warning) << "rBitFromMMT: " << static_cast<int>(rbFromMMT) << "/ from Delta: " << static_cast<int>(rbFromDelta);
					//}
					//assert(rbFromMMT >= rbFromDelta);
#endif

					this->serializeGap(bs, (int64_t)rbFromMMT - (int64_t)rbFromDelta);

#ifndef NDEBUG
					gaps.push_back((int64_t)rbFromMMT - (int64_t)rbFromDelta);
#endif

					if (rbFromDelta != 0)
					{
						bs << setw(rbFromDelta);
						Ty_ signMask = 0x1 << rbFromDelta - 1;

						while (!bItemItr->isEnd())
						{
							Ty_ value = (**bItemItr).get<Ty_>();
							if (value < 0)
							{
								value = abs_(value);
								value |= signMask;
							}
							bs << value;
							++(*bItemItr);
						}
					}

					++seqId;
				}
				++innerItr;
			}
		}

#ifndef NDEBUG
		//{
		//	std::stringstream ss;
		//	for (auto b : this->rBitFromMMT)
		//	{
		//		ss << static_cast<int>(b) << ", ";
		//	}
		//	BOOST_LOG_TRIVIAL(trace) << ss.str();
		//}
		//{
		//	std::stringstream ss;
		//	for (auto b : this->rBitFromDelta)
		//	{
		//		ss << static_cast<int>(b) << ", ";
		//	}
		//	BOOST_LOG_TRIVIAL(trace) << ss.str();
		//}

		//BOOST_LOG_TRIVIAL(debug) << "avg gaps: " << static_cast<double>(std::accumulate(gaps.begin(), gaps.end(), 0.0) / gaps.size());
		//BOOST_LOG_TRIVIAL(debug) << "avg from MMT: " << static_cast<double>(std::accumulate(rBitFromMMT.begin(), rBitFromMMT.end(), 0.0) / rBitFromMMT.size());
		//BOOST_LOG_TRIVIAL(debug) << "avg from Delta: " << static_cast<double>(std::accumulate(rBitFromDelta.begin(), rBitFromDelta.end(), 0.0) / rBitFromDelta.size());
#endif
	}

	template<typename Ty_>
	void deserialize(bstream& bs)
	{
		this->seDecode<Ty_>(bs);
	}

	template <typename Ty_>
	void seDecode(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);

		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);

		size_t seqId = 0;
		// Level 0
		{
			//BOOST_LOG_TRIVIAL(debug) << "MIN: " << this->min_;
			for (size_t band = 0; band <= numBandsInLevel; ++band, ++seqId)
			{
				this->deserializeBand<Ty_>(bs, myBlock, seqId, band, bandDims);
			}
		}

		this->deserializeChildLevelBand<Ty_>(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	template <typename Ty_>
	void deserializeBand(bstream& bs, pBlock myBlock,
						 const size_t seqId, const size_t bandId, dimension& bandDims)
	{
		size_t gap = this->deserializeGap(bs);
		bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];
		size_t rbFromDelta = rbFromMMT - gap;

#ifndef DEBUG
		//BOOST_LOG_TRIVIAL(trace) << "seqId: " << static_cast<int>(seqId) << ", setw: " << static_cast<int>(rbFromDelta);
		//BOOST_LOG_TRIVIAL(trace) << "fromMMT: " << static_cast<int>(rbFromMMT) << ", gap: " << static_cast<int>(gap) << ", rbDelta: " << static_cast<int>(rbFromDelta);
		//if (rbFromMMT < gap)
		//{
		//	BOOST_LOG_TRIVIAL(warning) << "deserializeBand";
		//	BOOST_LOG_TRIVIAL(warning) << "fromMMT: " << static_cast<int>(rbFromMMT) << ", gap: " << static_cast<int>(gap) << ", rbDelta: " << static_cast<int>(rbFromDelta);
		//}
		//assert(rbFromMMT >= gap);
#endif

		bs >> setw(rbFromDelta);
		Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
		Ty_ negativeMask = (Ty_)-1 ^ signMask;
		Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));

		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));

		//////////////////////////////
		// 01
		auto spOffset = bItemItr->seqPos() * sizeof(Ty_);
		auto itemCapa = bandDims.area();
		char* spData = (char*)this->getBuffer()->getData() + spOffset;

		if ((Ty_)-1 < 0)
		{
			// Ty_ has negative values
			for (int i = 0; i < itemCapa; ++i)
			{
				auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

				*pValue = 0;
				bs >> *pValue;
				if (*pValue & signMask)
				{
					*pValue &= negativeMask;
					*pValue *= -1;
					*pValue |= signBit;
				}
				//*pValue += this->min_;

				//Ty_ value = 0;
				//bs >> value;
				//if (value & signMask)
				//{
				//	value &= negativeMask;
				//	value *= -1;
				//	value |= signBit;	// for 128 (1000 0000)
				//}

				//memcpy(spData + this->tileOffset_[i], &value, sizeof(Ty_));
			}
		} else
		{
			// Ty_ only has positive values
			for (int i = 0; i < itemCapa; ++i)
			{
				auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

				*pValue = 0;
				bs >> *pValue;
			}
		}
		//////////////////////////////

		//////////////////////////////
		// 02
		//while (!bItemItr->isEnd())
		//{
		//	Ty_ value = 0;
		//	bs >> value;
		//	if (value & signMask)
		//	{
		//		value &= negativeMask;
		//		value *= -1;
		//		value |= signBit;	// for 128 (1000 0000)
		//	}

		//	(**bItemItr).set<Ty_>(value);
		//	++(*bItemItr);
		//}
		//////////////////////////////
	}

	template <typename Ty_>
	void deserializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		auto dSize = this->getDSize();
		for (size_t level = 1; level <= this->level_; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			mdItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + this->getChunkCoor() * innerSpace);
				//BOOST_LOG_TRIVIAL(trace) << "area: " << static_cast<int>(innerCoor.area());
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					dimension targetSp = getBandRange(band, bandDims * pow(2, level)).getSp() + innerItr.coor() * bandDims;
					dimension targetEp = targetSp + bandDims;

					size_t gap = this->deserializeGap(bs);
					bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];
					size_t rbFromDelta = rbFromMMT - gap;
#ifndef DEBUG
					//BOOST_LOG_TRIVIAL(trace) << targetSp.toString() << ", " << targetEp.toString();
					//BOOST_LOG_TRIVIAL(trace) << "seqId: " << static_cast<int>(seqId) << ", setw: " << static_cast<int>(rbFromDelta);
					//BOOST_LOG_TRIVIAL(trace) << "fromMMT: " << static_cast<int>(rbFromMMT) << ", gap: " << static_cast<int>(gap) << ", rbDelta: " << static_cast<int>(rbFromDelta);
					//if (rbFromMMT < gap)
					//{
					//	BOOST_LOG_TRIVIAL(warning) << "deserializeChildLevelBand: " << static_cast<int>(level) << ", " << seqId;
					//	BOOST_LOG_TRIVIAL(warning) << "fromMMT: " << static_cast<int>(rbFromMMT) << ", gap: " << static_cast<int>(gap) << ", rbDelta: " << static_cast<int>(rbFromDelta);
					//}
					//assert(rbFromMMT >= gap);
#endif

					auto bItemItr = inBlock->getItemRangeIterator(range(targetSp, targetEp));

					//////////////////////////////
					// 01
					auto spOffset = bItemItr->seqPos() * sizeof(Ty_);
					auto itemCapa = bandDims.area();
					char* spData = (char*)this->getBuffer()->getData() + spOffset;
					//////////////////////////////

					if (rbFromDelta == 0)
					{
						//////////////////////////////
						// 01
						Ty_ value = 0;
						for (size_type i = 0; i < itemCapa; ++i)
						{
							*((Ty_*)(spData + this->tileOffset_[i])) = 0;
						}
						//////////////////////////////

						//////////////////////////////
						// 02
						//while (!bItemItr->isEnd())
						//{
						//	(**bItemItr).set<Ty_>(0);
						//	++(*bItemItr);
						//}
						//////////////////////////////
					} else
					{
						bs >> setw(rbFromDelta);
						Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
						Ty_ negativeMask = (Ty_)-1 ^ signMask;
						Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));

						//////////////////////////////
						// 01
						if ((Ty_)-1 < 0)
						{
							for (size_type i = 0; i < itemCapa; ++i)
							{
								auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

								*pValue = 0;
								bs >> *pValue;
								if (*pValue & signMask)
								{
									*pValue &= negativeMask;
									*pValue *= -1;
									*pValue |= signBit;
								}
							}
						} else
						{
							for (size_type i = 0; i < itemCapa; ++i)
							{
								auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

								*pValue = 0;
								bs >> *pValue;
							}
						}
						//////////////////////////////

						//////////////////////////////
						// 02
						//while (!bItemItr->isEnd())
						//{
						//	Ty_ value = 0;
						//	bs >> value;
						//	if (value & signMask)
						//	{
						//		value &= negativeMask;
						//		value *= -1;
						//		value |= signBit;	// for 128 (1000 0000)
						//	}
						//
						//	(**bItemItr).set<Ty_>(value);
						//	++(*bItemItr);
						//}
						//////////////////////////////
					}

					++seqId;
				}
				++innerItr;
			}
		}
	}

public:
	inline void setTileOffset(std::vector<uint64_t>& offset)
	{
		this->tileOffset_ = offset;
	}
	inline void setMin(int64_t min)
	{
		this->min_ = min;
	}

	inline int64_t getMin()
	{
		return this->min_;
	}


protected:
	size_t level_;
	//chunkId sourceChunkId_;

public:
	std::vector<bit_cnt_type> rBitFromMMT;	// required bits from MMT node
	std::vector<bit_cnt_type> rBitFromDelta;	// required bits from delta array
	std::vector<uint64_t> tileOffset_;
	int64_t min_;
};

template <typename Ty_, typename mmtNode>
bit_cnt_type getRBitFromMMT(mmtNode node, bool hasNegative = true)
{
	if (node->order_ > 1)
	{
		if (SIGN(node->bMax_) > 0)
		{
			return node->bMax_ + static_cast<char>(hasNegative);
		}

		return abs_(node->bMin_) + static_cast<char>(hasNegative);
	}

	if (SIGN(node->bMax_) != SIGN(node->bMin_))
	{
		// bMax > 0
		return std::max((bit_cnt_type)node->bMax_, (bit_cnt_type)abs_(node->bMin_)) + static_cast<char>(hasNegative);
	}

	return std::max((bit_cnt_type)abs_(node->bMax_), (bit_cnt_type)abs_(node->bMin_)) + static_cast<char>(hasNegative);
}

template <typename Ty_>
bit_cnt_type getRBitFromMMT(Ty_ max, Ty_ min, bit_cnt_type order, bool hasNegative = true)
{
	//return msb<Ty_>(node->getMax<Ty_>() - node->getMin<Ty_>(), node->order_);
	return msb<Ty_>(max - min, order);
	//if (node->order_ > 1)
	//{
	//	if (SIGN(node->bMax_) > 0)
	//	{
	//		return node->bMax_ + static_cast<char>(hasNegative);
	//	}

	//	return abs_(node->bMin_) + static_cast<char>(hasNegative);
	//}

	//if (SIGN(node->bMax_) != SIGN(node->bMin_))
	//{
	//	// bMax > 0
	//	return std::max((bit_cnt_type)node->bMax_, (bit_cnt_type)abs_(node->bMin_)) + static_cast<char>(hasNegative);
	//}

	//return std::max((bit_cnt_type)abs_(node->bMax_), (bit_cnt_type)abs_(node->bMin_)) + static_cast<char>(hasNegative);
}
}		// core
}		// msdb
#endif	// _MSDB_SE_CHUNK_H_