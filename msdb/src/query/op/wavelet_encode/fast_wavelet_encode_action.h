#pragma once
#ifndef _MSDB_OP_FAST_WAVELET_ENCODE_ACTION_H_
#define _MSDB_OP_FAST_WAVELET_ENCODE_ACTION_H_

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
class fast_wavelet_encode_action : public opAction
{
public:
	fast_wavelet_encode_action();
	virtual ~fast_wavelet_encode_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

private:
	template <class Ty_>
	void attributeEncode(const concreteTy<Ty_>& type,
						 pArray outArr, pArray inArr, 
						 pAttributeDesc attrDesc, pQuery q)
	{
		pWavelet w = std::make_shared<wavelet>(this->waveletName_.c_str());
		size_t maxLevel = std::stoi(attrDesc->getParam(_STR_PARAM_WAVELET_LEVEL_));

		auto cItr = inArr->getChunkIterator(attrDesc->id_);
		auto dSize = cItr->dSize();
		auto cSize = cItr->getSeqEnd();

		while (!cItr->isEnd())
		{
			if (cItr->isExist())
			{
				// --------------------
				// TODO::PARALLEL
				auto convertedChunkList = this->chunkEncode<Ty_>((**cItr), w, maxLevel, q);
				// --------------------
				for (auto chk : convertedChunkList)
				{
					auto cid = outArr->chunkCoorToChunkId(chk->getDesc()->chunkCoor_);
					chk->setId(cid);
				}

				outArr->insertChunk(attrDesc->id_, convertedChunkList.begin(), convertedChunkList.end());
			}

			++(*cItr);
		}
	}

	template <>
	void attributeEncode(const concreteTy<bool>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery q)
	{}

	template <>
	void attributeEncode(const concreteTy<float>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery q)
	{}

	template <>
	void attributeEncode(const concreteTy<double>& type,
						 pArray outArr, pArray inArr,
						 pAttributeDesc attrDesc, pQuery q)
	{}

	template <class Ty_>
	std::list<pChunk> chunkEncode(pChunk inChunk,
								  pWavelet w, size_t maxLevel, pQuery q)
	{
		std::list<pChunk> outChunks;
		auto bItr = inChunk->getBlockIterator();
		while (!bItr->isEnd())
		{
			outChunks.push_back(this->blockEncode<Ty_>(inChunk, (**bItr)->getId(), w, maxLevel, q));
			++(*bItr);
		}

		return outChunks;
	}

	template <class Ty_>
	pChunk blockEncode(pChunk inChunk, blockId bid,
					   pWavelet w, size_t maxLevel, pQuery q)
	{
		auto inBlock = inChunk->getBlock(bid);
		auto inChunkDesc = inChunk->getDesc();
		auto inBlockDesc = inBlock->getDesc();
		auto blockCoor = inBlockDesc->blockCoor_;
		auto blockSpace = inChunkDesc->getBlockSpace();

		auto outChunkDesc = std::make_shared<chunkDesc>(*inChunkDesc);
		//outChunkDesc->id_ = outChunk->getId() * blockSpace.area() + bid;
		outChunkDesc->dims_ = outChunkDesc->blockDims_;
		outChunkDesc->chunkCoor_ = inChunkDesc->chunkCoor_ * blockSpace + blockCoor;
		outChunkDesc->sp_ = inChunkDesc->sp_ + inBlockDesc->getSp();		// csp + bsp
		outChunkDesc->ep_ = inChunkDesc->sp_ + inBlockDesc->getEp();		// csp + bep
		outChunkDesc->mSize_ = inBlockDesc->mSize_;

		auto outChunk = std::make_shared<wtChunk<Ty_>>(outChunkDesc);
		outChunk->makeAllBlocks();
		outChunk->setLevel(maxLevel);
		outChunk->bufferCopy(inBlock);

		for (size_t level = 0; level <= maxLevel; ++level)
		{
			range arrRange = outChunkDesc->blockDims_ / pow(2, level);
			this->levelEncode<Ty_>(outChunk, arrRange, w, level, q);
		#ifndef NDEBUG
			//BOOST_LOG_TRIVIAL(trace) << "wtEncode level: " << level;
			//outChunk->getBlock(0)->print();
		#endif
		}
		return outChunk;
	}

	template <class Ty_>
	void levelEncode(pChunk outChunk, range& arrRange,
					 pWavelet w, size_t level, pQuery q)
	{
		dimensionId dSize = outChunk->getDSize();
		for (dimensionId d = 0; d < dSize; ++d)
		{
			this->dimensionEncode<Ty_>(outChunk, arrRange, d, w, q);
		#ifndef NDEBUG
			//BOOST_LOG_TRIVIAL(trace) << "wtEncode dimension: " << d;
			//outChunk->getBlock(0)->print();
		#endif
		}
	}

	template <class Ty_>
	void dimensionEncode(pChunk outChunk,
						 range& arrRange, dimensionId basisDim,
						 pWavelet w, pQuery q)
	{
		Ty_* data = (Ty_*)outChunk->getBuffer()->getData();
		Ty_ signMask = 0x1 << (sizeof(Ty_) * CHAR_BIT - 1);

		// Set dimension params
		auto ep = arrRange.getEp();
		size_t area = ep.area();
		
		// Set input/output Bands
		size_t numInBands = pow(2, basisDim);
		size_t inBandSize = area / numInBands;
		size_t outBandSize = inBandSize / 2;

		size_t approxi_detail_offset = 1;
		size_t neighbor_offset = 1;
		for (dimensionId i = 0; i < basisDim; ++i)
		{
			neighbor_offset *= ep[i] / 2;		// previous dimensions are encoded before, dimension size decreases
		}
		approxi_detail_offset = neighbor_offset * (ep[basisDim] / 2);

		size_t numRow = ep[basisDim] / 2;
		for (dimensionId i = basisDim + 1; i < ep.size(); ++i)
		{
			numRow *= ep[i];		// Multiply rest dimensions
		}

		std::vector<Ty_*> bands;
		for (dimensionId i = 0; i < numInBands; ++i)
		{
			bands.push_back(data + (i * inBandSize));
		}

		std::vector<std::shared_ptr<std::vector<Ty_>>> outBands(numInBands * 2, nullptr);
		switch (basisDim)
		{
		case 0:
		{
			for (size_t i = 0; i < numInBands; ++i)
			{
				auto outA = std::make_shared<std::vector<Ty_>>(outBandSize);	// approximate
				auto outD = std::make_shared<std::vector<Ty_>>(outBandSize);	// detail

				for (size_t j = 0; j < outBandSize; ++j)
				{
					Ty_ x0 = data[j * 2];
					Ty_ x1 = data[j * 2 + 1];

					outD->at(j) = x1 - x0;
					if (outD->at(j) & signMask)
					{
						outA->at(j) = x0 - std::floor((Ty_)(~outD->at(j) + 1) / 2.0);
					}
					else
					{
						outA->at(j) = x0 + std::floor(outD->at(j) / 2.0);
					}
				}

				outBands[i] = outA;
				outBands[i + numInBands] = outD;
			}
			break;
		}
		default:
		{
			for (size_t i = 0; i < numInBands; ++i)
			{
				auto outA = std::make_shared<std::vector<Ty_>>(outBandSize);	// approximate
				auto outD = std::make_shared<std::vector<Ty_>>(outBandSize);	// detail

				for (size_t row = 0; row < numRow; ++row)
				{
					size_t k = row * neighbor_offset;
					size_t rowOffset = k * 2;

					for (size_t j = 0; j < neighbor_offset; ++j, ++k)
					{
						Ty_ x0 = data[j + rowOffset + i * inBandSize];
						Ty_ x1 = data[j + neighbor_offset + rowOffset + i * inBandSize];

						outD->at(k) = x1 - x0;
						if (outD->at(k) & signMask)
						{
							outA->at(k) = x0 - std::floor((Ty_)(~outD->at(k) + 1) / 2.0);
						}
						else
						{
							outA->at(k) = x0 + std::floor(outD->at(k) / 2.0);
						}
					}
				}

				outBands[i] = outA;
				outBands[i + numInBands] = outD;
			}
			break;
		}
		}

		for (size_t i = 0; i < outBands.size(); ++i)
		{
			memcpy(data + (i * outBandSize * sizeof(Ty_)), outBands[i]->data(), outBandSize * sizeof(Ty_));
		}
	}

private:
	/*
	*	┌───────────┐   ┌─────┬─────┐   ┌──┬──┬─────┐
	*	│           │   │     │     │   │ 0│ 1│     │
	*	│           │   │  0  │  1  │   ├──┼──┤  4  │
	*	│           │   │     │     │   │ 2│ 3│     │
	*	│     0     │ → ├─────┼─────┤ → ├──┴──┼─────┤
	*	│           │   │     │     │   │     │     │
	*	│           │   │  2  │  3  │   │  5  │  6  │
	*	│           │   │     │     │   │     │     │
	*	└───────────┘   └─────┴─────┘   └─────┴─────┘
	*      band 0          level 0         level 1
	*
	*	┌───────────┬───────────┐   ┌──┬──┬─────┬──┬──┬─────┐
	*	│           │           │   │ 0│ 1│     │ 7│ 8│     │
	*	│           │           │   ├──┼──┤  4  ├──┼──┤  11 │
	*	│           │           │   │ 2│ 3│     │ 9│10│     │
	*	│     0     │     1     │ → ├──┴──┼─────├──┴──┼─────┤
	*	│           │           │   │     │     │     │     │
	*	│           │           │   │  5  │  6  │  12 │  13 │
	*	│           │           │   │     │     │     │     │
	*	└───────────┴───────────┘   └─────┴─────┴─────┴─────┘
	*       band 0      band 1               level 1
	*/

public:
	virtual const char* name() override;
	std::string waveletName_;		// Do not delete this variable.
									// This for the testing various wavelet functions.
};
}
}
#endif		// _MSDB_OP_FAST_WAVELET_ENCODE_ACTION_H_