#pragma once
#ifndef _MSDB_OP_SE_COMPRESSION_ACTION_H_
#define _MSDB_OP_SE_COMPRESSION_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <array/block.h>
#include <array/flattenChunk.h>
#include <system/storageMgr.h>
#include <op/wavelet_encode/wtChunk.h>
#include "seChunk.h"
#include <compression/waveletUtil.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <util/logger.h>
#include <util/dataType.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
class se_compression_action : public opAction
{
public:
	se_compression_action();
	~se_compression_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

private:
	//Visitor
	template<typename Ty_>
	void compressAttribute(const concreteTy<Ty_>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc, pQuery qry)
	{
		size_t mSizeTotal = 0;
		size_t synopsisSizeTotal = 0;

		//========================================//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//
		size_t currentThreadId = 0;
		this->threadCreate();

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

		if((Ty_)-1 < 0)
		{
			hasNegative = true;
		}

		while (!cit->isEnd())
		{
			if (cit->isExist())
			{
				auto inChunk = std::static_pointer_cast<wtChunk<Ty_>>(**cit);
				auto cDesc = std::make_shared<chunkDesc>(*inChunk->getDesc());
				auto outChunk = std::static_pointer_cast<seChunk<Ty_>>(outArr->makeChunk(cDesc));

				////////////////////////////////////////
				// 1. Serialize::decompressChunk
				////////////////////////////////////////
				//this->compressChunk<Ty_>(arrId, outChunk, inChunk, mmtIndex, chunkDim, hasNegative, qry, currentThreadId);
				////////////////////////////////////////

				////////////////////////////////////////
				// 2. Parallel::decompressChunk
				////////////////////////////////////////
				io_service_->post(boost::bind(&se_compression_action::compressChunk<Ty_>, this,
											  arrId, outChunk, inChunk, mmtIndex, chunkDim, hasNegative, qry, currentThreadId));
				////////////////////////////////////////

			}

			++(*cit);
		}

		this->threadStop();
		this->threadJoin();

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//========================================//

		auto ocit = outArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);
		while (!ocit->isEnd())
		{
			auto outChunk = (**ocit);
			mSizeTotal += outChunk->getSerializedSize();
			synopsisSizeTotal += std::static_pointer_cast<seChunk<Ty_>>(outChunk)->getSynopsisSize();

			++(*ocit);
		}

		BOOST_LOG_TRIVIAL(info) << "Total Synopsis Size: " << synopsisSizeTotal << " Bytes" << std::endl
			<< "Total Save Chunk: " << mSizeTotal << " Bytes";
	}

	template<>
	void compressAttribute(const concreteTy<float>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc, pQuery qry)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "se_compress not support data compression for float"));
	}

	template<>
	void compressAttribute(const concreteTy<double>& type, pArray outArr, pArray inArr, pAttributeDesc attrDesc, pQuery qry)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "se_compress not support data compression for double"));
	}

	template<typename Ty_>
	void compressChunk(arrayId arrId,
					   std::shared_ptr<seChunk<Ty_>> outChunk,
					   std::shared_ptr<wtChunk<Ty_>> inChunk,
					   std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
					   dimension& sourceChunkDim, bool hasNegative,
					   pQuery qry,
					   const size_t parentThreadId)
	{
	#ifndef NDEBUG
		//BOOST_LOG_TRIVIAL(trace) << "Chunkcoor: " << chunkCoor.toString() << " / MMT NODE: " << mNode->toString<Ty_>();
		//assert(outChunk->rBitFromMMT <= sizeof(Ty_) * CHAR_BIT);
	#endif

		auto threadId = getThreadId() + 1;

		//========================================//
		qry->getTimer()->nextJob(threadId, this->name() + std::string("::Thread"), workType::COMPUTING, std::string("chunk::") + std::to_string(outChunk->getId()));
		//----------------------------------------//

		size_t dSize = inChunk->getDSize();

		outChunk->setLevel(inChunk->getLevel());
		//outChunk->setSourceChunkId(inChunk->getSourceChunkId());
		outChunk->bufferRef(inChunk);
		outChunk->makeAllBlocks();

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


		//----------------------------------------//
		qry->getTimer()->nextWork(threadId, workType::IO, std::string("chunk::") + std::to_string(outChunk->getId()));
		//----------------------------------------//

		auto attr = outChunk->getDesc()->attrDesc_;
		storageMgr::instance()->saveChunk(arrId, attr->id_, (outChunk)->getId(),
										  std::static_pointer_cast<serializable>(outChunk));

		//----------------------------------------//
		qry->getTimer()->pause(threadId);
		//========================================//
	}

	template <class Ty_>
	bit_cnt_type findRequiredBits(pBlock curBlock, const range& bandRange)
	{
		auto bItemItr = curBlock->getItemRangeIterator(bandRange);
		bit_cnt_type maxValueBits = 0;
		///// >> 220526 SIGNED MOD << /////
		//Ty_ mask = ((Ty_)-1) >> 1;
		Ty_ signMask = ((Ty_)1) << (sizeof(Ty_) * CHAR_BIT - 1);
		bool hasNegative = false;		// to treat -128, 128
		///// >> 220526 SIGNED ORI << /////
		// No code
		///// -- 220526 SIGNED END -- /////
#ifndef NDEBUG
		Ty_ maxValue = 0;
#endif

		while(!bItemItr->isEnd())
		{
			///// >> 220526 SIGNED MOD << /////
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
			///// >> 220526 SIGNED ORI << /////
//			Ty_ value = (**bItemItr).get<Ty_>();
//			bit_cnt_type valueBits = static_cast<bit_cnt_type>(msb<Ty_>(abs_(value)));
//			if (maxValueBits < valueBits)
//			{
//				maxValueBits = valueBits;
//#ifndef NDEBUG
//				maxValue = value;
//#endif
//			}
//			++(*bItemItr);
			///// -- 220526 SIGNED END -- /////
		}

#ifndef NDEBUG
		//BOOST_LOG_TRIVIAL(trace) << "Max value: " << static_cast<int>(maxValue) << ", maxValueBits: " << static_cast<int>(maxValueBits);
#endif
		if (!hasNegative && maxValueBits == 0)
		{
			return 0;				// no -128 and has only zeros
		}
		return maxValueBits + 1;	// In normal case, add sign bit
	}

	template <class Ty_>
	void findRequiredBitsForRootLevel(std::shared_ptr<seChunk<Ty_>> outChunk, pBlock outBlock,
									  std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
									  const dimension& bandDims, const size_t numBandsInLevel, 
									  const bool hasNegative)
	{
		auto chunkCoor = outChunk->getChunkCoor();
		auto blockLevel = mmtIndex->getBlockLevel();
		auto mNode = mmtIndex->getNode(chunkCoor, blockLevel);
		bit_cnt_type rbFromMMT = getRBitFromMMT<Ty_>(mNode, hasNegative);

		// TODO::Synopsis Delta Encoding
		//outChunk->setMin(mNode->getMin<Ty_>());
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
				// 220526
				//bit_cnt_type requiredBits = this->findRequiredBits<Ty_>(outBlock, getBandRange(band, bandDims)) + static_cast<char>(hasNegative);
				bit_cnt_type rbFromDelta = this->findRequiredBits<Ty_>(outBlock, getBandRange(band, bandDims));

				outChunk->rBitFromDelta.push_back(rbFromDelta);
				outChunk->rBitFromMMT.push_back(rbFromMMT);

			#ifndef NDEBUG
				//BOOST_LOG_TRIVIAL(trace) << "chunk: " << static_cast<int64_t>(outChunk->getId()) << ", level: " << static_cast<int>(0) << ", band: " << static_cast<int>(band);
				//BOOST_LOG_TRIVIAL(trace) << mNode->toString<Ty_>();
			#endif

						//assert(requiredBits <= fromMMT);
			}
		}
	}

	template <class Ty_>
	void findRequiredBitsForChildLevel(std::shared_ptr<seChunk<Ty_>> outChunk, pBlock outBlock,
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
				//bit_cnt_type rbFromMMT = std::max(static_cast<int64_t>(getRBitFromMMT<Ty_>(mNode, hasNegative) - (int64_t)level), static_cast<int64_t>(static_cast<char>(hasNegative)));
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
						// 220526
						//bit_cnt_type rbFromDelta = this->findRequiredBits<Ty_>(outBlock, range(targetSp, targetEp));
						//if(rbFromDelta > 0)
						//{
						//	rbFromDelta += static_cast<char>(hasNegative);
						//}

						outChunk->rBitFromDelta.push_back(rbFromDelta);
						outChunk->rBitFromMMT.push_back(rbFromMMT);

					#ifndef NDEBUG
						//BOOST_LOG_TRIVIAL(trace) << "level: " << static_cast<int>(level) << ", band: " << static_cast<int>(band);
						//if(band == 1)
						//{
						//	// band 1, 2, 3 has same mmtnode
						//	BOOST_LOG_TRIVIAL(trace) << mNode->toString<Ty_>();
						//}

						//if(rbFromDelta > rbFromMMT)
						//{
						//	BOOST_LOG_TRIVIAL(warning) << "rBitFromMMT: " << static_cast<int>(rbFromMMT) << "/ from Delta: " << static_cast<int>(rbFromDelta);
						//	BOOST_LOG_TRIVIAL(warning) << mNode->toString<Ty_>();
						//	BOOST_LOG_TRIVIAL(warning) << "chunk: " << static_cast<int64_t>(outChunk->getId())
						//		<< ", level: " << static_cast<int>(level) << ", band: " << static_cast<int>(band) 
						//		<< ", innerCoor: " << innerCoor.toString() << ", innerSpace: " << innerSpace.toString();
						//	BOOST_LOG_TRIVIAL(warning) << mNode->toString<Ty_>();
						//	BOOST_LOG_TRIVIAL(warning) << "range: " << targetSp.toString() << "~" << targetEp.toString();
						//	auto iit = outBlock->getItemRangeIterator(range(targetSp, targetEp));
						//	std::stringstream ss;
						//	while(!iit->isEnd())
						//	{
						//		ss << static_cast<int64_t>((**iit).get<Ty_>()) << ", ";
						//		++(*iit);
						//	}
						//	BOOST_LOG_TRIVIAL(debug) << ss.str();

						//	//assert(rbFromDelta <= rbFromMMT);
						//}
					#endif
					}
				}
				
				++innerItr;
			}
		}
	}

	//template <class Ty_>
	//void assertRequiredBits(bit_cnt_type rbFromDelta, bit_cnt_type rbFromMMT, const coor& chunkCoor, pMmtNode mNode, pBlock outBlock)
	//{
	//	//if (rbFromDelta > rbFromMMT)
	//	//{
	//	//	BOOST_LOG_TRIVIAL(warning) << "CompressChunk()=> requriedBits: " << static_cast<int64_t>(rbFromDelta) << ", "
	//	//		<< "rBitFromMMT: " << static_cast<int64_t>(rbFromMMT) << "\n"
	//	//		<< "MMT NODE: " << mNode->toString<Ty_>();
	//	//	BOOST_LOG_TRIVIAL(warning) << "Chunkcoor: " << chunkCoor.toString();
	//	//	outBlock->print();
	//	//}
	//	//assert(rbFromDelta <= rbFromMMT);
	//}
};
}	// core
}	// msdb
#endif		// _MSDB_OP_SE_COMPRESSION_ACTION_H_