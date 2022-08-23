#pragma once
#ifndef _MSDB_SE_CHUNK_H_
#define _MSDB_SE_CHUNK_H_

#include <pch.h>
#include <compression/compressionParam.h>
#include <array/flattenChunk.h>
#include <array/block.h>
#include <compression/waveletUtil.h>
#include <numeric>

namespace msdb
{
namespace core
{
template <typename Ty_>
class seChunk : public flattenChunk<Ty_>
{
public:
	seChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc), level_(0), rBitFromMMT(0), rBitFromDelta(0), min_(0), encodePrevValue_(0), encodeValueRepeatLevel_(0)
	{
		auto opParam = desc->getAttrDesc()->getOptionalParams();
		assert(opParam.find(_STR_PARAM_SE_LEVEL_) != opParam.end());

		try
		{
			this->level_ = std::stoi(opParam[_STR_PARAM_SE_LEVEL_]);
		}
		catch (...)
		{
			this->level_ = 0;
			_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(
				MSDB_EC_QUERY_ERROR, MSDB_ER_FAIL_TO_PARSE_PARAM,
				"seChunK::level: " + std::string(_STR_PARAM_SE_LEVEL_)));
		}
	}
	virtual ~seChunk()
	{

	}

public:
	// TODO::Use optional param
	inline size_t getLevel()
	{
		return this->level_;
	}
	//chunkId getSourceChunkId();

	// TODO::Use optional param
	inline void setLevel(size_t level)
	{
		this->level_ = level;
	}

	inline void setSourceChunkId(chunkId cid)
	{
		this->getDesc()->getAttrDesc()->setParam(std::string(_STR_PARAM_SOURCE_CHUNKID_), std::to_string(cid));
	}

	inline const chunkId& getSourceChunkId() const
	{
		return this->getDesc()->getAttrDesc()->getParam(std::string(_STR_PARAM_SOURCE_CHUNKID_));
	}
	//void setSourceChunkId(chunkId cid);

public:
	virtual void serialize(std::ostream& os) override
	{
		bstream bs;
		this->seEncode(bs);

		this->serializedSize_ = bs.capacity();
		this->getOutHeader()->serialize(os);
		os.write((char*)bs.data(), bs.capacity());
	}
	virtual void deserialize(std::istream& is) override
	{
		this->getHeader()->deserialize(is);
		this->updateFromHeader();

		bstream bs;
		bs.resize(this->serializedSize_);
		is.read((char*)bs.data(), this->serializedSize_);

		this->seDecode(bs);
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

	//template<typename Ty_>
	//void serialize(bstream& bs)
	//{
	//	this->seEncode<Ty_>(bs);
	//}
public:
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
		
		////////////////////////////////////////
		// TODO::Remove 53
		//if (this->getId() == 53)
		//{
		//	BOOST_LOG_TRIVIAL(debug) << "BlockDims: " << inBlockDims.toString();
		//	BOOST_LOG_TRIVIAL(debug) << "BandDims: " << bandDims.toString();
		//	BOOST_LOG_TRIVIAL(debug) << "NumBandInLevel: " << numBandsInLevel;

		//	auto size = this->rBitFromDelta.size();
		//	for (int i = 0; i < size; ++i)
		//	{
		//		BOOST_LOG_TRIVIAL(debug) << "[" << i << "]: " << static_cast<int64_t>(this->rBitFromMMT[i])
		//			<< "<-(" << static_cast<int64_t>(this->rBitFromMMT[i]- this->rBitFromDelta[i]) << ")->"
		//			<< static_cast<int64_t>(this->rBitFromDelta[i]);
		//	}
		//}
		////////////////////////////////////////
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
				if (band == 0)
				{
					size_t startPos = bs.getOutBitPos();
					this->serializeBand(bs, myBlock, seqId, band, bandDims);
					this->synopsisSize_ = bs.getOutBitPos() - startPos;
				}
				else
				{
					this->serializeBand(bs, myBlock, seqId, band, bandDims);
				}
				//this->serializeBand(bs, myBlock, seqId, band, bandDims);
			}

		#ifndef NDEBUG
			//auto synopsisSize = bs.capacity() - before;
			//BOOST_LOG_TRIVIAL(debug) << "Save Synopsis[" << this->desc_->id_ << "] : " << synopsisSize << " Bytes";
		#endif
		}

		//this->serializeChildLevelBand<Ty_>(bs, myBlock, seqId, bandDims, numBandsInLevel);
		this->serializeChildLevelBand(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	//template<typename Ty_>
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
		if (rbFromMMT == 0)
		{
			return;
		}

		this->serializeGap(bs, rbFromMMT - rbFromDelta);
		///// >> 220526 SIGNED MOD << /////
		if (rbFromDelta == 0)
		{
			return;
		}
		///// >> 220526 SIGNED ORI << /////
		// None
		///// -- 220526 SIGNED END -- /////

		bs << setw(rbFromDelta);							// TODO::rbFromDelta=0일 경우 8bit 모두 사용해서 값이 입력됨
		Ty_ signMask = 0x1 << rbFromDelta - 1;

		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		while (!bItemItr->isEnd())
		{
			//Ty_ value = (**bItemItr).get<Ty_>();
			//bs << (**bItemItr).get<Ty_>();
			//++(*bItemItr);

			Ty_ value = (**bItemItr).get<Ty_>();
			//value -= this->min_;

			///// >> 220526 SIGNED MOD << /////
			if (value & signMask)
			{
				value = (Ty_)(~value) + 1;
				value |= signMask;
			}
			///// >> 220526 SIGNED ORI << /////
			//if (value < 0)
			//{
			//	value = abs_(value);
			//	value |= signMask;
			//}
			///// -- 220526 SIGNED END -- /////

			bs << value;
			++(*bItemItr);
		}
	}

	//template <typename Ty_>
	void serializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		auto dSize = this->getDSize();

		this->encodePrevValue_ = 0;
		this->encodeValueRepeatLevel_ = 1;

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
					if (rbFromMMT)
					{
						this->serializeGap(bs, (int64_t)rbFromMMT - (int64_t)rbFromDelta);
					#ifndef NDEBUG
						gaps.push_back((int64_t)rbFromMMT - (int64_t)rbFromDelta);
					#endif

						if (rbFromDelta)
						{
							bs << setw(rbFromDelta);
							Ty_ signMask = 0x1 << rbFromDelta - 1;

							std::list<Ty_> valueList;
							while (!bItemItr->isEnd())
							{
								Ty_ value = (**bItemItr).get<Ty_>();

								if (this->encodeValueRepeatLevel_ > 0)
								{
									//BOOST_LOG_TRIVIAL(debug) << "Level: " << this->encodeValueRepeatLevel_;
									if (value == this->encodePrevValue_)
									{
										valueList.push_back(value);
										if (valueList.size() == pow(2, this->encodeValueRepeatLevel_))
										{
											//BOOST_LOG_TRIVIAL(debug) << static_cast<int64_t>(value) << "=equal=" << static_cast<int64_t>(this->encodePrevValue_);
											//BOOST_LOG_TRIVIAL(debug) << "Size: " << pow(2, this->encodeValueRepeatLevel_);
											bs << setw(1);
											bs << 0x1;		// set True
											valueList.clear();
											++(this->encodeValueRepeatLevel_);
										}
									}
									else
									{
										//BOOST_LOG_TRIVIAL(debug) << static_cast<int64_t>(value) << "<diff>" << static_cast<int64_t>(this->encodePrevValue_);
										bs << setw(1);
										bs << 0x0;		// set False

										// out the values in List
										bs << setw(rbFromDelta);
										for (Ty_ v : valueList)
										{
											//BOOST_LOG_TRIVIAL(debug) << "bs << v: " << static_cast<int64_t>(v);
											if (v & signMask)
											{
												v = (Ty_)(~v) + 1;
												v |= signMask;
											}
											bs << v;
										}
										valueList.clear();

										//BOOST_LOG_TRIVIAL(debug) << "bs << v: " << static_cast<int64_t>(value);
										// out the current value
										this->encodePrevValue_ = value;
										if (value & signMask)
										{
											value = (Ty_)(~value) + 1;
											value |= signMask;
										}
										bs << value;
										--(this->encodeValueRepeatLevel_);
									}
								}
								else
								{
									//BOOST_LOG_TRIVIAL(debug) << "Level: " << this->encodeValueRepeatLevel_;
									//BOOST_LOG_TRIVIAL(debug) << "bs<<value: " << static_cast<int64_t>(value);

									if (value == this->encodePrevValue_)
									{
										//BOOST_LOG_TRIVIAL(debug) << "Equal";
										++(this->encodeValueRepeatLevel_);
									}
									else
									{
										//BOOST_LOG_TRIVIAL(debug) << "Diff";
										--(this->encodeValueRepeatLevel_);
									}
									this->encodePrevValue_ = value;

									// out the current value
									bs << setw(rbFromDelta);
									if (value & signMask)
									{
										value = (Ty_)(~value) + 1;
										value |= signMask;
									}
									bs << value;
								}

								////////////////////////////////////////
								//if (value == this->encodePrevValue_)
								//{
								//	valueList.push_back(value);

								//	if (this->encodeValueRepeatLevel_ > 0)
								//	{
								//		if (valueList.size() == pow(2, this->encodeValueRepeatLevel_))
								//		{
								//			bs << setw(1);
								//			bs << 0x1;		// set True
								//			valueList.clear();
								//			++(this->encodeValueRepeatLevel_);
								//		}
								//	}
								//	else
								//	{
								//		assert(valueList.size() == 1);

								//		// out the current value
								//		bs << setw(rbFromDelta);
								//		if (value & signMask)
								//		{
								//			value = (Ty_)(~value) + 1;
								//			value |= signMask;
								//		}
								//		bs << value;
								//		++(this->encodeValueRepeatLevel_);
								//	}
								//}
								//else
								//{
								//	if (this->encodeValueRepeatLevel_ > 0)
								//	{
								//		bs << setw(1);
								//		bs << 0x0;		// set False
								//	}

								//	this->encodePrevValue_ = value;
								//	--(this->encodeValueRepeatLevel_);

								//	// out the values in List
								//	bs << setw(rbFromDelta);
								//	for (Ty_ v : valueList)
								//	{
								//		if (v & signMask)
								//		{
								//			v = (Ty_)(~v) + 1;
								//			v |= signMask;
								//		}
								//		bs << v;
								//	}
								//	valueList.clear();

								//	// out the current value
								//	if (value & signMask)
								//	{
								//		value = (Ty_)(~value) + 1;
								//		value |= signMask;
								//	}
								//	bs << value;
								//}

								++(*bItemItr);
							}

							if (valueList.size())
							{
								bs << setw(1);
								bs << 0x1;		// set True
								valueList.clear();
								++(this->encodeValueRepeatLevel_);
							}
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

	//template<typename Ty_>
	//void deserialize(bstream& bs)
	//{
	//	this->seDecode<Ty_>(bs);
	//}

	//template <typename Ty_>
public:
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
				this->deserializeBand(bs, myBlock, seqId, band, bandDims);
			}
		}

		//this->deserializeChildLevelBand<Ty_>(bs, myBlock, seqId, bandDims, numBandsInLevel);
		this->deserializeChildLevelBand(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	//template <typename Ty_>
	void deserializeBand(bstream& bs, pBlock myBlock,
						 const size_t seqId, const size_t bandId, dimension& bandDims)
	{
		bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];
		if (rbFromMMT == 0)
		{
			// Do nothing, the buffer should be initialized in 0(zeor).
			return;
		}

		size_t gap = this->deserializeGap(bs);
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

			///// >> 220526 SIGNED MOD << /////
		if (rbFromDelta == 0)
		{
			// Do nothing, the buffer should be initialized in 0(zeor).
			return;
		}

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

		// Ty_ has negative values
		for (int i = 0; i < itemCapa; ++i)
		{
			auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

			*pValue = 0;
			bs >> *pValue;
			if (*pValue & signMask)
			{
				*pValue = (Ty_)(~*pValue) + 1;
				*pValue |= signMask;
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

		///// >> 220526 SIGNED ORI << /////
		//bs >> setw(rbFromDelta);
		//Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
		//Ty_ negativeMask = (Ty_)-1 ^ signMask;
		//Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));

		//auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));

		////////////////////////////////
		//// 01
		//auto spOffset = bItemItr->seqPos() * sizeof(Ty_);
		//auto itemCapa = bandDims.area();
		//char* spData = (char*)this->getBuffer()->getData() + spOffset;

		//if ((Ty_)-1 < 0)
		//{
		//	// Ty_ has negative values
		//	for (int i = 0; i < itemCapa; ++i)
		//	{
		//		auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

		//		*pValue = 0;
		//		bs >> *pValue;
		//		if (*pValue & signMask)
		//		{
		//			*pValue &= negativeMask;
		//			*pValue *= -1;
		//			*pValue |= signBit;
		//		}
		//		//*pValue += this->min_;

		//		//Ty_ value = 0;
		//		//bs >> value;
		//		//if (value & signMask)
		//		//{
		//		//	value &= negativeMask;
		//		//	value *= -1;
		//		//	value |= signBit;	// for 128 (1000 0000)
		//		//}

		//		//memcpy(spData + this->tileOffset_[i], &value, sizeof(Ty_));
		//	}
		//}
		//else
		//{
		//	// Ty_ only has positive values
		//	for (int i = 0; i < itemCapa; ++i)
		//	{
		//		auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

		//		*pValue = 0;
		//		bs >> *pValue;
		//	}
		//}
		//////////////////////////////
		///// -- 220526 SIGNED END -- /////


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

	//template <typename Ty_>
	void deserializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		this->encodePrevValue_ = 0;
		this->encodeValueRepeatLevel_ = 1;

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

					bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];

					if (rbFromMMT)
					{
						int64_t gap = this->deserializeGap(bs);
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

						//if (rbFromDelta == 0)
						//{
						//	//////////////////////////////
						//	// 01
						//	Ty_ value = 0;
						//	for (size_t i = 0; i < itemCapa; ++i)
						//	{
						//		*((Ty_*)(spData + this->tileOffset_[i])) = 0;
						//	}
						//	//////////////////////////////
						//}
						if (rbFromDelta)
						{
							///// >> 220526 SIGNED MOD << /////
							bs >> setw(rbFromDelta);
							Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
							Ty_ negativeMask = (Ty_)-1 ^ signMask;
							Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));
							char repeatFlag = 0;

							//////////////////////////////
							// 01
							for (size_t i = 0; i < itemCapa; ++i)
							{
								if (this->encodeValueRepeatLevel_ > 0)
								{
									//BOOST_LOG_TRIVIAL(debug) << "Level: " << this->encodeValueRepeatLevel_;

									bs >> setw(1);
									bs >> repeatFlag;

									if (repeatFlag)
									{
										for (int j = 0; j < pow(2, this->encodeValueRepeatLevel_) && i < itemCapa; ++j, ++i)
										{
											auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
											*pValue = this->encodePrevValue_;
										}

										//BOOST_LOG_TRIVIAL(debug) << "=equal=" << static_cast<int64_t>(this->encodePrevValue_);
										//BOOST_LOG_TRIVIAL(debug) << "Size: " << pow(2, this->encodeValueRepeatLevel_);

										++(this->encodeValueRepeatLevel_);
										--i;
									}
									else
									{
										bs >> setw(rbFromDelta);

										Ty_ value = 0;
										do
										{
											auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

											*pValue = 0;
											bs >> *pValue;
											if (*pValue & signMask)
											{
												*pValue = (Ty_)(~*pValue) + 1;
												*pValue |= signMask;
											}
											value = *pValue;
											++i;
											//if (value == this->encodePrevValue_)
											//{
											//	BOOST_LOG_TRIVIAL(debug) << "Equal";
											//}
											//BOOST_LOG_TRIVIAL(debug) << "bs >> v: " << static_cast<int64_t>(value);
										} while (value == this->encodePrevValue_ && i < itemCapa);
										--i;

										//BOOST_LOG_TRIVIAL(debug) << static_cast<int64_t>(value) << "<diff>" << static_cast<int64_t>(this->encodePrevValue_);

										this->encodePrevValue_ = value;
										--(this->encodeValueRepeatLevel_);
									}
								}
								else
								{
									//BOOST_LOG_TRIVIAL(debug) << "Level: " << this->encodeValueRepeatLevel_;
									
									bs >> setw(rbFromDelta);

									auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
									*pValue = 0;
									bs >> *pValue;
									if (*pValue & signMask)
									{
										*pValue = (Ty_)(~*pValue) + 1;
										*pValue |= signMask;
									}

									//BOOST_LOG_TRIVIAL(debug) << "bs>>value: " << static_cast<int64_t>(*pValue);
									
									if (*pValue == this->encodePrevValue_)
									{
										//BOOST_LOG_TRIVIAL(debug) << "Equal";
										++(this->encodeValueRepeatLevel_);
									}
									else
									{
										//BOOST_LOG_TRIVIAL(debug) << "Diff";
										--(this->encodeValueRepeatLevel_);
									}
									this->encodePrevValue_ = (Ty_)*pValue;
								}
							}

							//////////////////////////////
							///// >> 220526 SIGNED ORI << /////
							//bs >> setw(rbFromDelta);
							//Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
							//Ty_ negativeMask = (Ty_)-1 ^ signMask;
							//Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));

							////////////////////////////////
							//// 01
							//if ((Ty_)-1 < 0)
							//{
							//	for (size_t i = 0; i < itemCapa; ++i)
							//	{
							//		auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

							//		*pValue = 0;
							//		bs >> *pValue;
							//		if (*pValue & signMask)
							//		{
							//			*pValue &= negativeMask;
							//			*pValue *= -1;
							//			*pValue |= signBit;
							//		}
							//	}
							//} else
							//{
							//	for (size_t i = 0; i < itemCapa; ++i)
							//	{
							//		auto pValue = (Ty_*)(spData + this->tileOffset_[i]);

							//		*pValue = 0;
							//		bs >> *pValue;
							//	}
							//}
							////////////////////////////////
							///// -- 220526 SIGNED END -- /////

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
						else
						{
							// Do nothing
						}
					}
					else
					{
						// Do nothing
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

	inline size_t getSynopsisSize()
	{
		return this->synopsisSize_;
	}

protected:
	size_t level_;
	size_t synopsisSize_;
	//chunkId sourceChunkId_;

public:
	std::vector<bit_cnt_type> rBitFromMMT;	// required bits from MMT node
	std::vector<bit_cnt_type> rBitFromDelta;	// required bits from delta array
	std::vector<uint64_t> tileOffset_;
	int64_t min_;

public:
	Ty_ encodePrevValue_;
	//int64_t encodeValueRepeat_;
	int64_t encodeValueRepeatLevel_;
};

template <>
class seChunk<double> : public flattenChunk<double>
{
public:
	seChunk(pChunkDesc desc)
		: flattenChunk<double>(desc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "seChunk: double"));
	}

public:
	inline void setLevel(size_t level)
	{
	}
	inline void setSourceChunkId(chunkId cid)
	{
	}
	inline size_t getLevel()
	{
		return 0;
	}
	inline const chunkId& getSourceChunkId() const
	{
		return 0;
	}
};

template <>
class seChunk<float> : public flattenChunk<float>
{
public:
	seChunk(pChunkDesc desc)
		: flattenChunk<float>(desc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "seChunk: float"));
	}

public:
	inline void setLevel(size_t level)
	{
	}
	inline void setSourceChunkId(chunkId cid)
	{
	}
	inline size_t getLevel()
	{
		return 0;
	}
	inline const chunkId& getSourceChunkId() const
	{
		return 0;
	}
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

template <typename Ty_, typename mmtNode>
bit_cnt_type getRBitFromMMTForDetailBlock(mmtNode node, bool hasNegative = true)
{
	Ty_ min = boost::any_cast<Ty_>(node->min_);
	Ty_ max = boost::any_cast<Ty_>(node->max_);
	return msb<Ty_>(max - min);
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

//////////////////////////////
// seChunkFactory
//
// To make concreteType of seChunk
//
template <typename Ty_>
class seChunkFactory : public chunkFactory
{
public:
	seChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for seChunkFacotry
//
class seChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	seChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<seChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// seChunkType
//
class seChunkType : public chunkType
{
public:
	seChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "seChunk";
	}
};
}		// core
}		// msdb

#include "seChunk.hpp"

#endif	// _MSDB_SE_CHUNK_H_