#pragma once
#ifndef _MSDB_OP_SE_DECOMPRESSION_ACTION_H_
#define _MSDB_OP_SE_DECOMPRESSION_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <array/flattenChunk.h>
#include <system/storageMgr.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/se_compression/seChunk.h>
#include <compression/waveletUtil.h>
#include <query/opAction.h>
#include <index/mmt.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
class se_decompression_action : public opAction
{
public:
	se_decompression_action();
	~se_decompression_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery qry) override;

private:
	//pSeChunk makeInChunk(std::shared_ptr<wavelet_encode_array> arr, pAttributeDesc attrDesc,
	//					  chunkId cid, coor chunkCoor);

	template <typename Ty_>
	void decompressAttribute(const concreteTy<Ty_>& type,
							 pArray outArr, pArray inArr,
							 pAttributeDesc attrDesc, pQuery qry)
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
		auto mmtIndex = std::static_pointer_cast<MinMaxTreeImpl<Ty_>>(arrIndex);

		size_t maxLevel = std::stoi(attrDesc->getParam(_STR_PARAM_SE_LEVEL_));
		auto outAttrDesc = outArr->getDesc()->getAttrDescs()->at(attrDesc->getId());
		outAttrDesc->setParam(_STR_PARAM_SE_LEVEL_, std::to_string(maxLevel));

		std::vector<uint64_t> offsets = this->getSeqOffInBand<Ty_>(outArr, maxLevel);		// To fast access band in serialized memory
		auto cit = inArr->getChunkIterator(attrDesc->id_, iterateMode::ALL);

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//
		size_t currentThreadId = 0;
		this->threadCreate();

		while (!cit->isEnd())
		{
			if(cit->isExist())
			{
				chunkId cid = cit->seqPos();
				coor chunkCoor = cit->coor();
				auto inChunk = std::static_pointer_cast<seChunk<Ty_>>(**cit);
				
				// TODO::Check block coordinate, size, position, id etc...
				// Flatten -> (shuffled) -> wavelet -> seacow
				// 
				// make chunk
				// auto inChunk = this->makeInChunk(outArr, attrDesc, cid, chunkCoor);		// TODO::Remove makeInChunk()

				inChunk->setTileOffset(offsets);
				auto cDesc = inChunk->getDesc();
				auto outChunk = std::static_pointer_cast<wtChunk<Ty_>>(outArr->makeChunk(std::make_shared<chunkDesc>(*cDesc)));
				//outChunk->bufferAlloc();
				//inChunk->bufferRef(outChunk);		// outChunk should hold the buffer -> used latter
				
				inChunk->makeAllBlocks();
				outChunk->makeAllBlocks();

				inChunk->setLevel(maxLevel);
				outChunk->setLevel(maxLevel);
				// TODO::Copy buffer, etc...
				
				//this->decompressChunk<Ty_>(inChunk, outChunk, qry, outArr, attrId, maxLevel, mmtIndex, currentThreadId);
				io_service_->post(boost::bind(&se_decompression_action::decompressChunk<Ty_>, this, 
											  inChunk, outChunk, qry, outArr, attrId, maxLevel, mmtIndex, currentThreadId));

				outChunk->bufferCopy(inChunk);
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

	template <>
	void decompressAttribute(const concreteTy<float>& type, 
							 pArray inArr, pArray outArr,
							 pAttributeDesc attrDesc, pQuery qry)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "se_decompress not support data compression for float"));
	}

	template <>
	void decompressAttribute(const concreteTy<double>& type,
							 pArray inArr, pArray outArr,
							 pAttributeDesc attrDesc, pQuery qry)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "se_decompress not support data compression for double"));
	}

	template <typename Ty_>
	std::vector<uint64_t> getSeqOffInBand(pArray outArr, const size_t& maxLevel)
	{
		dimension blockDims = outArr->getDesc()->getDimDescs()->getChunkDims();
		size_t blockCapa = blockDims.area();
		dimension bandDims = blockDims / std::pow(2, maxLevel + 1);
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
	void decompressChunk(std::shared_ptr<seChunk<Ty_>> inChunk, std::shared_ptr<wtChunk<Ty_>> outChunk, 
						 pQuery qry, pArray outArr, attributeId attrId,
						 const size_t maxLevel,
						 std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex, const size_t parentThreadId)
	{
		auto threadId = getThreadId();

		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::COMPUTING);
		//----------------------------------------//
		//auto maxLevel = outArr->getMaxLevel();
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

		//outChunk->setLevel(inChunk->getLevel());
		//outChunk->replaceBlockBitmap(inChunk->getBlockBitmap());
		//outChunk->makeBlocks();
		//outChunk->bufferCopy(inChunk);
		outChunk->setSerializedSize(inChunk->getSerializedSize());

		//----------------------------------------//
		qry->getTimer()->pause(threadId);
		//========================================//
	}

	template <typename Ty_>
	void requiredBitsFindingForChunk(std::shared_ptr<seChunk<Ty_>> inChunk, 
						 std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
						 const size_t maxLevel,
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
	void findRequiredBitsForRootLevel(std::shared_ptr<seChunk<Ty_>> inChunk,
									  std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
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
	void findRequiredBitsForChildLevel(std::shared_ptr<seChunk<Ty_>> inChunk,
									   std::shared_ptr<MinMaxTreeImpl<Ty_>> mmtIndex,
									   const size_t maxLevel,
									   const size_t numBandsInLevel,
									   const bool hasNegative)
	{
		auto dSize = inChunk->getChunkCoor().size();
		for (size_t level = 1; level <= maxLevel; ++level)
		{
			auto innerSize = pow(2, level);
			dimension innerSpace = dimension(dSize, innerSize);
			mdItr innerItr(innerSpace);
			while (!innerItr.isEnd())
			{
				coor innerCoor(innerItr.coor() + inChunk->getChunkCoor() * innerSpace);
				auto mNode = mmtIndex->getNode(innerCoor, mmtIndex->getBlockLevel() - level);
				bit_cnt_type rbFromMMT = std::max(static_cast<int64_t>(getRBitFromMMT<Ty_>(mNode, hasNegative) - (int64_t)level), static_cast<int64_t>(static_cast<char>(hasNegative)));
				for (size_t band = 1; band <= numBandsInLevel; ++band)
				{
					inChunk->rBitFromMMT.push_back(rbFromMMT);

//#ifndef NDEBUG
//					BOOST_LOG_TRIVIAL(trace) << "level: " << level << ", band: " << band;
//					BOOST_LOG_TRIVIAL(trace) << mNode->toString<Ty_>();
//#endif
				}

				++innerItr;
			}
		}
	}
};
}		// core
}		// msdb
#endif		// _MSDB_OP_SE_DECOMPRESSION_ACTION_H_