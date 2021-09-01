#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ACTION_H_
#define _MSDB_OP_WAVELET_ENCODE_ACTION_H_

#include <pch.h>
#include <compression/wavelet.h>
#include <compression/wtChunk.h>
#include <query/opAction.h>
#include <util/math.h>
#include <vector>
#include <list>

namespace msdb
{
namespace core
{
class wavelet_encode_action : public opAction
{
public:
	wavelet_encode_action();
	virtual ~wavelet_encode_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

private:
	template <class Ty_>
	void attributeEncode(pArray outArr, pArray inArr, pAttributeDesc attrDesc,
						 pWavelet w, size_t maxLevel, pQuery q)
	{
		auto cItr = inArr->getChunkIterator();
		auto dSize = cItr->dSize();
		auto cSize = cItr->getSeqEnd();

		while(!cItr->isEnd())
		{
			// --------------------
			// TODO::PARALLEL
			auto convertedChunkList = this->chunkEncode<Ty_>((**cItr), w, maxLevel, q);
			// --------------------
			for(auto chk : convertedChunkList)
			{
				auto cid = outArr->getChunkIdFromChunkCoor(chk->getDesc()->chunkCoor_);
				chk->setId(cid);
			}

			outArr->insertChunk(attrDesc->id_, convertedChunkList.begin(), convertedChunkList.end());
			++(*cItr);
		}
	}

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

		pWtChunk outChunk = std::make_shared<wtChunk>(outChunkDesc);
		outChunk->makeAllBlocks();
		outChunk->setLevel(maxLevel);
		outChunk->bufferCopy(inBlock);

		for (size_t level = 0; level <= maxLevel; ++level)
		{
			coorRange arrRange = outChunkDesc->blockDims_ / pow(2, level);
			this->levelEncode<Ty_>(outChunk, arrRange, w, level, q);
#ifndef NDEBUG
			//BOOST_LOG_TRIVIAL(trace) << "wtEncode level: " << level;
			//outChunk->getBlock(0)->print();
#endif
		}
		return outChunk;
	}

	template <class Ty_>
	void levelEncode(pChunk outChunk, coorRange& arrRange, 
					 pWavelet w, size_t level, pQuery q)
	{
		dimensionId dSize = outChunk->getDSize();
		for(dimensionId d = 0; d < dSize; ++d)
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
						 coorRange& arrRange, dimensionId basisDim,
						 pWavelet w, pQuery q)
	{
		size_t length = arrRange.getEp()[basisDim];
		size_t halfLength = length / 2;
		bool oddLength = (length % 2) != 0;
		bool singleConvFlag = (length % 4) >= 2;
		size_t rows = arrRange.getEp().area() / length;

		auto approximateRange = arrRange;
		auto detailRange = arrRange;
		approximateRange.getEp()[basisDim] /= 2;
		detailRange.getSp()[basisDim] = approximateRange.getEp()[basisDim];

		assert(approximateRange.getEp()[basisDim] == length / 2);
		assert(detailRange.getSp()[basisDim] == approximateRange.getEp()[basisDim]);

		auto iit = outChunk->getBlock(0)->getItemRangeIterator(arrRange);
		auto ait = outChunk->getBlock(0)->getItemRangeIterator(approximateRange);
		auto dit = outChunk->getBlock(0)->getItemRangeIterator(detailRange);

		iit->setBasisDim(basisDim);
		ait->setBasisDim(basisDim);
		dit->setBasisDim(basisDim);

		iit->moveToStart();
		ait->moveToStart();
		dit->moveToStart();

		// convert to ty
		Ty_* row = new Ty_[length];

		for(size_t r = 0; r < rows; ++r)
		{
			for(size_t ai = 0, di = halfLength; ai < halfLength; ++ai, ++di)
			{
				Ty_ x0 = (**iit).get<Ty_>();
				++(*iit);
				Ty_ x1 = (**iit).get<Ty_>();
				++(*iit);

				row[di] = x1 - x0;
				row[ai] = x0 + std::floor(row[di] / 2.0);
			}

			for(size_t ai = 0, di = halfLength; ai < halfLength; ++ai, ++di)
			{
				(**ait).set<Ty_>(static_cast<Ty_>(row[ai]));
				++(*ait);
				(**dit).set<Ty_>(static_cast<Ty_>(row[di]));
				++(*dit);
			}

			if(oddLength)
			{
				++(*iit);
				++(*ait);
				++(*dit);
			}
		}

		delete[] row;
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
}		// core
}		// msdb
#endif		// _MSDB_OP_WAVELET_ENCODE_ACTION_H_