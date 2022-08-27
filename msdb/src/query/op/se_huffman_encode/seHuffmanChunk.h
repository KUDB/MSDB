#pragma once
#ifndef _MSDB_SEHUFFMANCHUNK_H_
#define _MSDB_SEHUFFMANCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>
#include <op/se_compression/seChunk.h>
#include <compression/huffmanCode.h>
#include <compression/fixedHuffmanCoder.h>
#include "seHuffmanBlock.h"

namespace msdb
{
namespace core
{
template <typename Ty_>
class seHuffmanChunk : public seChunk<Ty_>
{
public:
	using lzwCodeType = std::uint16_t;

public:
	seHuffmanChunk(pChunkDesc desc)
		: seChunk<Ty_>(desc)
	{
	}

	virtual ~seHuffmanChunk()
	{
	}

public:
	virtual pBlock makeBlock(const blockId bId) override
	{
		assert(this->blockCapacity_ > bId);
		if (this->blocks_[bId] == nullptr)
		{
			// Make new one
			auto desc = this->getBlockDesc(bId);
			auto blockObj = std::make_shared<seHuffmanBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
		}

		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override
	{
		bstream bs;
		this->seHuffmanEncode(bs);

		this->serializedSize_ = bs.capacity();
		this->getOutHeader()->serialize(os);
		os.write((char*)bs.data(), bs.capacity());
	}
	virtual void deserialize(std::istream& is) override
	{
		_MSDB_TRY_BEGIN
		{
			this->getHeader()->deserialize(is);
			this->updateFromHeader();

			bstream bs;
			bs.resize(this->serializedSize_);
			is.read((char*)bs.data(), this->serializedSize_);

			this->seHuffmanDecode(bs);
		}
		_MSDB_CATCH_EXCEPTION(e)
		{
			BOOST_LOG_TRIVIAL(error) << "CATCH:: Load Chunk : STD::EXCEPTION";
			BOOST_LOG_TRIVIAL(error) << e.what();
		}
	}

	void seHuffmanEncode(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);

		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);

		size_t seqId = 0;
		// Level 0
		{
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
			}
		}

		this->serializeChildLevelBand(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	void serializeBand(bstream& bs, pBlock myBlock,
					   size_t seqId, size_t bandId, dimension& bandDims)
	{
		bit_cnt_type rbFromDelta = this->rBitFromDelta[seqId];
		bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];

		if (rbFromMMT == 0)
		{
			return;
		}

		this->serializeGap(bs, rbFromMMT - rbFromDelta);
		if (rbFromDelta == 0)
		{
			return;
		}

		bs << setw(rbFromDelta);
		Ty_ signMask = 0x1 << rbFromDelta - 1;

		auto bItemItr = myBlock->getItemRangeIterator(getBandRange(bandId, bandDims));
		//auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];
		size_t beforeBitPos = bs.getOutBitPos();

		while (!bItemItr->isEnd())
		{
			Ty_ value = (**bItemItr).get<Ty_>();

			if (value & signMask)
			{
				value = (Ty_)(~value) + 1;
				value |= signMask;
			}

			//coder->encode(bs, &value, 1);
			bs << value;
			++(*bItemItr);
		}

		auto after = bs.getOutBitPos();
		//BOOST_LOG_TRIVIAL(debug) << "Bits: " << bs.getOutBitPos() - beforeBitPos;
	}

	void serializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		this->encodePrevValue_ = 0;
		this->encodeValueRepeatLevel_ = 1;

	#ifndef NDEBUG
		std::vector<int64_t> gaps;
	#endif

		//BOOST_LOG_TRIVIAL(debug) << "SE HUFFMAN CHUNK - serializeChildLevelBand";
		auto dSize = this->getDSize();

		for (size_t level = 1; level <= this->level_; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			mdItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + this->getChunkCoor() * innerSpace);
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					dimension targetSp = getBandRange(band, bandDims * pow(2, level)).getSp() + innerItr.coor() * bandDims;
					dimension targetEp = targetSp + bandDims;
					auto bItemItr = inBlock->getItemRangeIterator(range(targetSp, targetEp));

					bit_cnt_type rbFromDelta = this->rBitFromDelta[seqId];
					bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];

					//BOOST_LOG_TRIVIAL(debug) << "Lv: " << level << ", Bd: " << band << ", Block: " << bs.getOutBlockPos() << ", Bit: " << bs.getOutBitPosInBlock();
					//BOOST_LOG_TRIVIAL(debug) << "Block: " << bs.getOutLastBlock();

					if (rbFromMMT != 0)
					{
						this->serializeGap(bs, (int64_t)rbFromMMT - (int64_t)rbFromDelta);
					#ifndef NDEBUG
						gaps.push_back((int64_t)rbFromMMT - (int64_t)rbFromDelta);
					#endif
						
						if (rbFromDelta)
						{
							auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];

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
											coder->encode(bs, &v, 1);
											//bs << v;
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
										coder->encode(bs, &value, 1);
										//bs << value;
										--(this->encodeValueRepeatLevel_);
									}
								}
								else
								{
									if (value == this->encodePrevValue_)
									{
										++(this->encodeValueRepeatLevel_);
									}
									else
									{
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
									coder->encode(bs, &value, 1);
									//bs << value;
								}

								++(*bItemItr);
							}

							if (valueList.size())
							{
								bs << setw(1);
								bs << 0x1;		// set True
								valueList.clear();
								++(this->encodeValueRepeatLevel_);
							}
							//BOOST_LOG_TRIVIAL(debug) << "Bits: " << bs.getOutBitPos() - beforeBitPos;
						}
					}

					++seqId;
				}
				++innerItr;
			}
		}
	}

	void seHuffmanDecode(bstream& bs)
	{
		pBlock myBlock = this->blocks_.at(0);

		size_t dSize = this->getDSize();
		size_t numBandsInLevel = std::pow(2, dSize) - 1;
		dimension inBlockDims = this->getDesc()->getBlockDims();
		dimension bandDims = inBlockDims / std::pow(2, this->level_ + 1);

		size_t seqId = 0;
		// Level 0
		{
			for (size_t band = 0; band <= numBandsInLevel; ++band, ++seqId)
			{
				_MSDB_TRY_BEGIN
				{
					this->deserializeBand(bs, myBlock, seqId, band, bandDims);

				}	_MSDB_CATCH_EXCEPTION(e)
				{
					BOOST_LOG_TRIVIAL(error) << "CATCH:: Load Chunk : STD::EXCEPTION";
					BOOST_LOG_TRIVIAL(error) << e.what();
				}
			}
		}

		this->deserializeChildLevelBand(bs, myBlock, seqId, bandDims, numBandsInLevel);
	}

	void deserializeBand(bstream& bs, pBlock myBlock,
						 const size_t seqId, const size_t bandId, dimension& bandDims)
	{
		bit_cnt_type rbFromMMT = this->rBitFromMMT[seqId];
		if (rbFromMMT == 0)
		{
			// Do nothing, the buffer should be initialized in 0(zeor).
			return;
		}

		int64_t gap = this->deserializeGap(bs);
		assert(gap <= 64);
		size_t rbFromDelta = rbFromMMT - gap;

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
		//assert(rbFromDelta <= 16);
		//auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];

		//////////////////////////////
		// 01
		auto spOffset = bItemItr->seqPos() * sizeof(Ty_);
		auto itemCapa = bandDims.area();
		char* spData = (char*)this->getBuffer()->getData() + spOffset;

		//size_t tempBufferSize = std::min({ (size_t)(rbFromDelta * itemCapa * sizeof(Ty_) / (double)CHAR_BIT * 2) + 2, (size_t)(bs.capacity() - bs.getInBlockPos()) });
		//bstream bsHuffman;
		//bsHuffman.resize(tempBufferSize);
		//memcpy(bsHuffman.data(), bs.data() + bs.getInBlockPos() * bs.getBlockBytes(), tempBufferSize);
		//bsHuffman.jumpBits(bs.getInBitPosInBlock());

		//Ty_* huffmanDecoded = new Ty_[itemCapa];
		//auto readBits = coder->decode(static_cast<void*>(huffmanDecoded), itemCapa, bsHuffman);
		//bs.jumpBits(readBits);
		//BOOST_LOG_TRIVIAL(debug) << "Bits: " << readBits;

		// Ty_ has negative values
		for (int i = 0; i < itemCapa; ++i)
		{
			auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
			//*pValue = huffmanDecoded[i];
			bs >> *pValue;

			if (*pValue & signMask)
			{
				*pValue = (Ty_)(~*pValue) + 1;
				*pValue |= signBit;
			}
		}

		//delete[] huffmanDecoded;
		//////////////////////////////
	}

	void deserializeChildLevelBand(bstream& bs, pBlock inBlock, size_t seqId, dimension& bandDims, size_t numBandsInLevel)
	{
		this->encodePrevValue_ = 0;
		this->encodeValueRepeatLevel_ = 1;

	#ifndef NDEBUG
		std::vector<int64_t> gaps;
	#endif

		//BOOST_LOG_TRIVIAL(debug) << "SE HUFFMAN CHUNK - deserializeChildLevelBand";
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
					#ifndef NDEBUG
						gaps.push_back(gap);
					#endif
						//BOOST_LOG_TRIVIAL(debug) << "Gap: " << (int64_t)gap << ", After Block: " << bs.getInFrontBlock();
						if (gap > 64)
						{
							assert(gap <= 64);
						}
						size_t rbFromDelta = rbFromMMT - gap;

						//BOOST_LOG_TRIVIAL(debug) << "delta: " << static_cast<int64_t>(rbFromDelta) << " / mmt: " << static_cast<int64_t>(rbFromMMT) << " / gap: " << static_cast<int64_t>(gap);

						auto bItemItr = inBlock->getItemRangeIterator(range(targetSp, targetEp));
						//////////////////////////////
						// 01
						auto spOffset = bItemItr->seqPos() * sizeof(Ty_);
						auto itemCapa = bandDims.area();
						char* spData = (char*)this->getBuffer()->getData() + spOffset;
						//////////////////////////////

						if (rbFromDelta)
						{
							//assert(rbFromDelta <= 16);
							auto coder = this->fixedHuffmanCoders[sizeof(Ty_)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(Ty_) * CHAR_BIT) })];

							bs >> setw(rbFromDelta);
							Ty_ signMask = (Ty_)(0x1 << (rbFromDelta - 1));
							Ty_ negativeMask = (Ty_)-1 ^ signMask;
							Ty_ signBit = (Ty_)(0x1 << (sizeof(Ty_) * CHAR_BIT - 1));
							char repeatFlag = 0;

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
											Ty_ v = 0;
											auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
											v = this->getHuffmanDecode(bs, coder, rbFromDelta);
											*pValue = v;
											//bs >> *pValue;
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

									Ty_ v = 0;
									auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
									v = this->getHuffmanDecode(bs, coder, rbFromDelta);
									*pValue = v;
									//bs >> *pValue;
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
							// 01
							//if ((Ty_)-1 < 0)
							//{
							//	for (size_t i = 0; i < itemCapa; ++i)
							//	{
							//		auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
							//		*pValue = huffmanDecoded[i];

							//		if (*pValue & signMask)
							//		{
							//			*pValue &= negativeMask;
							//			*pValue *= -1;
							//			*pValue |= signBit;
							//		}
							//	}
							//}
							//else
							//{
							//	for (size_t i = 0; i < itemCapa; ++i)
							//	{
							//		auto pValue = (Ty_*)(spData + this->tileOffset_[i]);
							//		*pValue = huffmanDecoded[i];
							//	}
							//}
							//////////////////////////////

							//delete[] huffmanDecoded;
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

	Ty_ getHuffmanDecode(bstream& bs, iFixedHuffmanCoder* coder, bit_cnt_type rbFromDelta)
	{
		static size_t itemCapa = 1;

		// Read more buffer
		size_t tempBufferSize = std::min({ (size_t)(rbFromDelta * itemCapa * sizeof(Ty_) / (double)CHAR_BIT * 2) + 2, (size_t)(bs.capacity() - bs.getInBlockPos()) });
		bstream bsHuffman;
		bsHuffman.resize(tempBufferSize);
		memcpy(bsHuffman.data(), bs.data() + bs.getInBlockPos() * bs.getBlockBytes(), tempBufferSize);
		bsHuffman.jumpBits(bs.getInBitPosInBlock());

		//Ty_ huffmanDecoded = new Ty_[itemCapa];
		//auto readBits = coder->decode(static_cast<void*>(huffmanDecoded), itemCapa, bsHuffman);
		////BOOST_LOG_TRIVIAL(debug) << "Bits: " << readBits;
		//bs.jumpBits(readBits);

		Ty_ huffmanDecoded = 0;
		auto readBits = coder->decode(static_cast<void*>(&huffmanDecoded), itemCapa, bsHuffman);
		//BOOST_LOG_TRIVIAL(debug) << "Bits: " << readBits;
		bs.jumpBits(readBits);

		return huffmanDecoded;
	}

private:
	static std::vector<std::vector<iFixedHuffmanCoder*>> fixedHuffmanCoders;
};

template <>
class seHuffmanChunk<double> : public seChunk<double>
{
public:
	seHuffmanChunk(pChunkDesc desc)
		: seChunk<double>(desc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "seHuffmanChunk: double"));
	}
};

template <>
class seHuffmanChunk<float> : public seChunk<float>
{
public:
	seHuffmanChunk(pChunkDesc desc)
		: seChunk<float>(desc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "seHuffmanChunk: float"));
	}
};

//////////////////////////////
// seHuffmanChunkFactory
//
// To make concreteType of seHuffmanChunk
//
template <typename Ty_>
class seHuffmanChunkFactory : public chunkFactory
{
public:
	seHuffmanChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};		// seHuffmanChunkFactory

//////////////////////////////
// Factory constructor for seHuffmanChunkFacotry
//
class seHuffmanChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	seHuffmanChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<seHuffmanChunkFactory<Ty_>>();
	}
};		// seHuffmanChunkFactoryBuilder

//////////////////////////////
// seHuffmanChunkType
//
class seHuffmanChunkType : public chunkType
{
public:
	seHuffmanChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "seHuffmanChunk";
	}
};		// seHuffmanChunkType
}		// core
}		// msdb

#include "seHuffmanChunk.hpp"

#endif	// _MSDB_SEHUFFMANCHUNK_H_