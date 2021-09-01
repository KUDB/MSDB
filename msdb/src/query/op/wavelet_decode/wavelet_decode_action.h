#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_ACTION_H_
#define _MSDB_OP_WAVELET_DECODE_ACTION_H_

#include <pch.h>
#include <array/blockChunk.h>
#include <compression/wavelet.h>
#include <query/opAction.h>
#include <util/math.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
class wavelet_decode_action : public opAction
{
public:
	wavelet_decode_action();
	virtual ~wavelet_decode_action();

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);
	virtual const char* name() override;

private:
	template <class Ty_>
	void attributeDecode(pArray outArr, pArray inArr, pAttributeDesc attrDesc,
						 pWavelet w, size_t maxLevel, pQuery qry)
	{
		//========================================//
		qry->getTimer()->nextWork(0, workType::COMPUTING);
		//----------------------------------------//
		auto outChunkBitmap = this->inferBottomUpBitmap(outArr->getDesc(), inArr->getChunkBitmap());
		outArr->replaceChunkBitmap(outChunkBitmap);
		auto ocItr = outArr->getChunkIterator(iterateMode::ALL);

		//----------------------------------------//
		qry->getTimer()->nextWork(0, workType::PARALLEL);
		//----------------------------------------//
		size_t currentThreadId = 0;
		this->threadCreate(_MSDB_ACTION_THREAD_NUM_);

		while (!ocItr->isEnd())
		{
			if(ocItr->isExist())
			{
				pChunk outChunk = outArr->makeChunk(attrDesc->id_, ocItr->seqPos());
				outChunk->bufferAlloc();

				io_service_->post(boost::bind(&wavelet_decode_action::parallelChunkDecode<Ty_>, this,
								  outArr, outChunk, inArr->getChunkIterator(),
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
		while(!ocItr->isEnd())
		{
			if(ocItr->isExist() && (**ocItr)->getBlockBitmap()->isEmpty())
			{
				outArr->freeChunk(ocItr->seqPos());
			}
			++(*ocItr);
		}
	}

	template <typename Ty_>
	void parallelChunkDecode(pArray outArr, pChunk outChunk, pChunkIterator icItr,
							 const coor outChunkCoor,
							 pWavelet w, const size_t maxLevel, pQuery qry, const size_t parentThreadId)
	{
		auto threadId = getThreadId();

		//========================================//
		qry->getTimer()->nextJob(threadId, this->name(), workType::COMPUTING);
		//----------------------------------------//
		
		auto chunkDims = outArr->getDesc()->getDimDescs()->getChunkDims();
		auto blockSpace = outArr->getDesc()->getDimDescs()->getBlockSpace();

		bool isOutChunkEmpty = true;
		std::vector<pChunk> chunks;
		//pBitmap blockBitmap = std::make_shared<bitmap>(blockSpace.area(), false);
		auto blockBitmap = outChunk->getBlockBitmap();
		auto blockItr = outChunk->getBlockIterator();
		//coorItr bSpaceItr(blockSpace);

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
			} else
			{
				//BOOST_LOG_TRIVIAL(debug) << "null";
				chunks.push_back(nullptr);
				blockBitmap->setNull(blockItr->seqPos());
			}
			//++bSpaceItr;
			++(*blockItr);
		}

		if (!isOutChunkEmpty)
		{
#ifndef NDEBUG
			BOOST_LOG_TRIVIAL(trace) << "[" << outChunk->getId() << "]: chunk exist";
#endif
			//isChunkExist->at(outChunk->getId()) = true;

			this->chunkDecode<Ty_>(chunks, outChunk, outChunkCoor, chunkDims,
								   blockSpace, w, maxLevel, qry);
		} else
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
			//std::cout << "Level Decode" << std::endl;
			//outBlock->print();
		}
	}

	template <class Ty_>
	void levelDecode(pBlock outBlock, pWavelet w, const size_t level, pQuery q)
	{
		dimensionId dSize = outBlock->getDSize();
		for (dimensionId d = dSize - 1; d != (dimensionId)-1; --d)
		{
			coorRange arrRange = outBlock->getDesc()->dims_ / pow(2, level);
			this->dimensionDecode<Ty_>(outBlock, arrRange, d, w, q);
		}
	}

	template <class Ty_>
	void dimensionDecode(pBlock outBlock,
						 const coorRange& encodeRange, const dimensionId& basisDim,
						 pWavelet w, pQuery q)
	{
		size_t length = encodeRange.getEp()[basisDim];
		size_t halfLength = length / 2;
		bool oddLength = (length % 2) != 0;
		bool singleConvFlag = (length % 4) >= 2;
		size_t rows = encodeRange.getEp().area() / length;

		auto approximateRange = encodeRange;
		auto detailRange = encodeRange;
		approximateRange.getEp()[basisDim] /= 2;
		detailRange.getSp()[basisDim] = approximateRange.getEp()[basisDim];

#ifndef NDEBUG
		assert(approximateRange.getEp()[basisDim] == length / 2);
		assert(detailRange.getSp()[basisDim] == approximateRange.getEp()[basisDim]);
#endif

		auto iit = outBlock->getItemRangeIterator(encodeRange);
		auto ait = outBlock->getItemRangeIterator(approximateRange);
		auto dit = outBlock->getItemRangeIterator(detailRange);

		iit->setBasisDim(basisDim);
		ait->setBasisDim(basisDim);
		dit->setBasisDim(basisDim);

		iit->moveToStart();
		ait->moveToStart();
		dit->moveToStart();

		// convert to ty
		Ty_* row = new Ty_[length];

		for (size_t r = 0; r < rows; ++r)
		{
			for (size_t i = 0; i < halfLength * 2; i += 2)
			{
				Ty_ y0 = (**ait).get<Ty_>();
				Ty_ y1 = (**dit).get<Ty_>();

				row[i] = y0 - std::floor(y1 / 2.0);
				row[i + 1] = y1 + row[i];

				++(*ait);
				++(*dit);
			}

			// halfLength * 2 -> Even number
			for (size_t i = 0; i < halfLength * 2; ++i)
			{
				(**iit).set<Ty_>(static_cast<Ty_>(row[i]));
				++(*iit);
			}

			if (oddLength)
			{
				++(*iit);
				++(*ait);
				++(*dit);
			}
		}

		delete[] row;
	}

	pBitmapTree inferBottomUpBitmap(pArrayDesc aDesc, cpBitmap fromBottom)
	{
		dimension chunkSpace = aDesc->getDimDescs()->getChunkSpace();
		dimension blockSpace = aDesc->getDimDescs()->getBlockSpace();
		dimension seChunkSpace = chunkSpace * blockSpace;

		auto inChunkItr = coorItr(seChunkSpace);
		auto outChunkItr = coorItr(chunkSpace);
		auto outBlockItr = coorItr(blockSpace);

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
#endif		// _MSDB_OP_WAVELET_DECODE_ACTION_H_