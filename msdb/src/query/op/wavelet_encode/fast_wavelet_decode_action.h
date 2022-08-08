#pragma once
#ifndef _MSDB_OP_FAST_WAVELET_DECODE_ACTION_H_
#define _MSDB_OP_FAST_WAVELET_DECODE_ACTION_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/wavelet.h>
#include <query/opAction.h>
#include <util/math.h>
#include <util/threadUtil.h>
#include <util/dataType.h>
#include "wtChunk.h"

namespace msdb
{
namespace core
{
class fast_wavelet_decode_action : public opAction
{
public:
	fast_wavelet_decode_action();
	virtual ~fast_wavelet_decode_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);
	virtual const char* name() override;

private:
	template <class Ty_>
	void attributeDecode(const concreteTy<Ty_>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery qry)
	{
		pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());
		size_t maxLevel = std::stoi(attrDesc->getParam(_STR_PARAM_WAVELET_LEVEL_));

		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//
		auto outChunkBitmap = this->inferBottomUpBitmap(outArr->getDesc(), inArr->getChunkBitmap());
		outArr->replaceChunkBitmap(outChunkBitmap);
		auto ocItr = outArr->getChunkIterator(attrDesc->id_, iterateMode::ALL);

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//
		size_t currentThreadId = 0;
		this->threadCreate();

		while (!ocItr->isEnd())
		{
			if (ocItr->isExist())
			{
				pChunk outChunk = outArr->makeChunk(attrDesc->id_, ocItr->seqPos());
				outChunk->bufferAlloc();

				io_service_->post(boost::bind(&fast_wavelet_decode_action::parallelChunkDecode<Ty_>, this,
											  outArr, outChunk, inArr->getChunkIterator(attrDesc->id_),
											  ocItr->coor(),
											  w, maxLevel, qry, currentThreadId));
			}

			++(*ocItr);
		}

		this->threadStop();
		this->threadJoin();

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//========================================//

		// Free empty chunks
		ocItr->moveToStart();
		while (!ocItr->isEnd())
		{
			if (ocItr->isExist() && (**ocItr)->getBlockBitmap()->isEmpty())
			{
				outArr->freeChunk(attrDesc->id_, ocItr->seqPos());
			}
			++(*ocItr);
		}
	}

	template <>
	void attributeDecode(const concreteTy<bool>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery qry)
	{}

	template <>
	void attributeDecode(const concreteTy<float>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery qry)
	{}

	template <>
	void attributeDecode(const concreteTy<double>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery qry)
	{}

	template <typename Ty_>
	void parallelChunkDecode(pArray outArr, pChunk outChunk, pChunkIterator icItr,
							 const coor outChunkCoor,
							 pWavelet w, const size_t maxLevel, pQuery qry, const size_t parentThreadId)
	{
		auto threadId = getThreadId();

		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::IO);
		//----------------------------------------//

		auto chunkDims = outArr->getDesc()->getDimDescs()->getChunkDims();
		auto blockSpace = outArr->getDesc()->getDimDescs()->getBlockSpace();

		bool isOutChunkEmpty = true;
		std::vector<pChunk> chunks;
		//pBitmap blockBitmap = std::make_shared<bitmap>(blockSpace.area(), false);
		auto blockBitmap = outChunk->getBlockBitmap();
		auto blockItr = outChunk->getBlockIterator();
		//mdItr bSpaceItr(blockSpace);

		// Bring all chunks belong to an outChunk
		while (!blockItr->isEnd())
		{
			auto blockCoor = blockItr->coor();
			auto inChunkCoor = outChunkCoor * blockSpace + blockCoor;

			icItr->moveTo(inChunkCoor);
			//BOOST_LOG_TRIVIAL(debug) << inChunkCoor.toString();
			//if(icItr->isExist() && (**icItr) == nullptr)
			//{
			//	BOOST_LOG_TRIVIAL(debug) << "exist but null";
			//}

			if (icItr->isExist() && (**icItr) != nullptr)
			{
				//BOOST_LOG_TRIVIAL(debug) << "exist";
			#ifndef NDEBUG
				assert((*icItr)->getDesc()->chunkCoor_ == inChunkCoor);
			#endif

				outChunk->makeBlock(blockItr->seqPos());
				//blockBitmap->setExist(bSpaceItr.seqPos());
				chunks.push_back(**icItr);
				isOutChunkEmpty = false;
			}
			else
			{
				//BOOST_LOG_TRIVIAL(debug) << "null";
				chunks.push_back(nullptr);
				blockBitmap->setNull(blockItr->seqPos());
			}
			//++bSpaceItr;
			++(*blockItr);
		}

		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::COMPUTING);
		//========================================//

		if (!isOutChunkEmpty)
		{
		#ifndef NDEBUG
			BOOST_LOG_TRIVIAL(trace) << "[" << outChunk->getId() << "]: chunk exist";
		#endif
			//isChunkExist->at(outChunk->getId()) = true;

			this->chunkDecode<Ty_>(chunks, outChunk, outChunkCoor, chunkDims,
								   blockSpace, w, maxLevel, qry);
		}
		else
		{
		#ifndef NDEBUG
			BOOST_LOG_TRIVIAL(trace) << "[" << outChunk->getId() << "]: chunk empty";
		#endif
			//isChunkExist->at(outChunk->getId()) = false;
			//outArr->freeChunk(outChunk->getId());		// Occur errors: multi-thread race condition
														// Bitmap: true / chunk: null
		}

		//----------------------------------------//
		qry->getTimer()->pause(threadId);
		//========================================//
	}

	template <class Ty_>
	void chunkDecode(std::vector<pChunk>& inChunkList, pChunk outChunk,
					 const coor& outChunkCoor, const dimension& chunkDims,
					 const dimension& blockSpace,
					 pWavelet w, const size_t maxLevel, pQuery q)
	{
		auto obItr = outChunk->getBlockIterator();

		for (blockId bid = 0; bid < inChunkList.size(); ++bid)
		{
			if (obItr->isExist())
			{
				auto inChunk = inChunkList[obItr->seqPos()];
				coor chunkCoor = inChunk->getChunkCoor();
				coor blockCoor = chunkCoor % blockSpace;
				obItr->moveTo(blockCoor);

				this->blockDecode<Ty_>((**obItr), inChunk->getBlock(0), w, maxLevel, q);
			}
			++(*obItr);
		}
	}

	template <class Ty_>
	void blockDecode(pBlock outBlock, pBlock inBlock,
					 pWavelet w, const size_t maxLevel, pQuery q)
	{
		// Copy data from inBlock
		outBlock->copy(inBlock);
		for (size_t level = maxLevel; level != (size_t)-1; --level)
		{
			this->levelDecode<Ty_>(outBlock, w, level, q);
		}
	}

	template <class Ty_>
	void levelDecode(pBlock outBlock, pWavelet w, const size_t level, pQuery q)
	{
		dimensionId dSize = outBlock->getDSize();
		for (dimensionId d = dSize - 1; d != (dimensionId)-1; --d)
		{
			range arrRange = outBlock->getDesc()->dims_ / pow(2, level);
			this->dimensionDecode<Ty_>(outBlock, arrRange, d, w, q);
		}
	}

	template <class Ty_>
	void dimensionDecode(pBlock outBlock,
						 const range& encodeRange, const dimensionId& basisDim,
						 pWavelet w, pQuery q)
	{
		Ty_* data = (Ty_*)outBlock->getBuffer()->getData();

		
		Ty_ signMask = 0x1 << (sizeof(Ty_) * CHAR_BIT - 1);

		// Set dimension params
		auto ep = encodeRange.getEp();
		size_t basisDimSize = ep[basisDim];
		size_t area = ep.area();

		std::vector<Ty_> inData(data, data + area);


		// Set input/output Bands
		size_t numOutBands = pow(2, basisDim);
		size_t numInBands = numOutBands * 2;
		size_t inBandSize = area / numInBands;
		size_t outBandSize = inBandSize * 2;

		std::shared_ptr<Ty_[]> outData(new Ty_[numOutBands * outBandSize * sizeof(Ty_)]);
		std::vector<Ty_*> inBands;
		for (size_t i = 0; i < numInBands; ++i)
		{
			inBands.push_back(data + (i * inBandSize));		// Use pointer to avoid copy
		}
		std::vector<Ty_*> outBands(numOutBands, nullptr);
		for (size_t i = 0; i < numOutBands; ++i)
		{
			outBands[i] = outData.get() + outBandSize * i;
		}

		// Set offsets
		size_t approxi_detail_offset = 1;
		size_t neighbor_offset = 1;
		for (dimensionId i = 0; i < basisDim; ++i)
		{
			neighbor_offset *= ep[i] / 2;		// previous dimensions are encoded before, dimension size decreases
		}
		approxi_detail_offset = neighbor_offset * (basisDimSize / 2);

		// Set row
		size_t numRow = basisDimSize / 2;
		for (dimensionId i = basisDim + 1; i < ep.size(); ++i)
		{
			numRow *= ep[i];		// Multiply rest dimensions
		}

		switch (basisDim)
		{
		case 0:
		{
			for (size_t i = 0; i < numOutBands; ++i)
			{
				auto outBand = outBands[i];

				auto inA = inBands[i];					// approximate
				auto inD = inBands[i + numOutBands];	// detail

				for (size_t j = 0; j < inBandSize; ++j)
				{
					Ty_ y0 = inA[j];
					Ty_ y1 = inD[j];

					Ty_ x0 = 0;
					if (y1 & signMask)
					{
						x0 = y0 + std::floor((Ty_)(~y1 + 1) / 2.0);
					}
					else
					{
						x0 = y0 - std::floor(y1 / 2.0);
					}
					outBand[j * 2] = x0;
					outBand[j * 2 + 1] = y1 + x0;
				}
			}
			break;
		}
		default:
		{
			for (size_t i = 0; i < numOutBands; ++i)
			{
				auto outBand = outBands[i];

				auto inA = inBands[i];					// approximate
				auto inD = inBands[i + numOutBands];	// detail

				for (size_t row = 0; row < numRow; ++row)
				{
					size_t k = row * neighbor_offset;
					size_t rowOffset = k * 2;

					for (size_t j = 0; j < neighbor_offset; ++j, ++k)
					{
						Ty_ y0 = inA[k];
						Ty_ y1 = inD[k];
						Ty_ x0 = 0;

						if (y1 & signMask)
						{
							x0 = y0 + std::floor((Ty_)(~y1 + 1) / 2.0);
						}
						else
						{
							x0 = y0 - std::floor(y1 / 2.0);
						}
						outBand[j + rowOffset] = x0;
						outBand[j + neighbor_offset + rowOffset] = y1 + x0;
					}
				}
			}

			//for (size_t i = 0; i < numOutBands; ++i)
			//{
			//	size_t curABandPos = i * inBandSize;
			//	size_t curDBandPos = (i + numOutBands) * inBandSize;
			//	std::transform(std::begin(inData) + curABandPos, std::begin(inData) + curABandPos + inBandSize,
			//				   std::begin(inData) + curDBandPos, std::begin(inData) + curDBandPos + inBandSize,
			//				   [&signMask](Ty_ y0, Ty_ y1)
			//				   {
			//					   if (y1 & signMask)
			//					   {
			//						   return y0 + std::floor((Ty_)(~y1 + 1) / 2.0);
			//					   }
			//					   else
			//					   {
			//						   return y0 - std::floor(y1 / 2.0);
			//					   }
			//				   });

			//	std::transform(std::begin(inData) + curDBandPos, std::begin(inData) + curDBandPos + inBandSize,
			//				   std::begin(inData) + curABandPos, std::begin(inData) + curABandPos + inBandSize,
			//				   [](Ty_ y1, Ty_ x0)
			//				   {
			//					   return y1 + x0;
			//				   });
			//}
			break;
		}
		}

		memcpy(data, outData.get(), numOutBands * outBandSize * sizeof(Ty_));
	}

	pBitmapTree inferBottomUpBitmap(pArrayDesc aDesc, cpBitmap fromBottom)
	{
		dimension chunkSpace = aDesc->getDimDescs()->getChunkSpace();
		dimension blockSpace = aDesc->getDimDescs()->getBlockSpace();
		dimension seChunkSpace = chunkSpace * blockSpace;

		auto inChunkItr = mdItr(seChunkSpace);
		auto outChunkItr = mdItr(chunkSpace);
		auto outBlockItr = mdItr(blockSpace);

		pBitmapTree outBitmap = std::make_shared<bitmapTree>(chunkSpace.area(), false);

		while (!inChunkItr.isEnd())
		{
			if (fromBottom->isExist(inChunkItr.seqPos()))
			{
				dimension blockCoor = inChunkItr.coor() % blockSpace;
				dimension chunkCoor = (inChunkItr.coor() - blockCoor) / blockSpace;	// Divide operator for dimension is rounded up by default
				outChunkItr.moveTo(chunkCoor);
				outBlockItr.moveTo(blockCoor);

				outBitmap->setExist(outChunkItr.seqPos());

				if (!outBitmap->hasChild(outChunkItr.seqPos()))
				{
					outBitmap->makeChild(outChunkItr.seqPos(), blockSpace.area(), false);
				}

				auto blockBitmap = outBitmap->getChild(outChunkItr.seqPos());
				blockBitmap->setExist(outBlockItr.seqPos());
			}

			++inChunkItr;
		}

		return outBitmap;
	}

public:
	std::string waveletName_;	// This for the testing various wavelet functions.
};
}		// core
}		// msdb
#endif		// _MSDB_OP_FAST_WAVELET_DECODE_ACTION_H_