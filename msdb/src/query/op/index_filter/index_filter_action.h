#pragma once
#ifndef _MSDB_INDEX_FILTER_ACTION_H_
#define _MSDB_INDEX_FILTER_ACTION_H_

#include <pch.h>
#include <array/block.h>
#include <query/opAction.h>
#include <parse/predicate.h>

namespace msdb
{
namespace core
{
class index_filter_action : public opAction
{
public:
	index_filter_action();
	virtual ~index_filter_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

private:
	template<class Ty_>
	void attributeFilter(pArray outArr, pArray inArr, pAttributeDesc attrDesc, pPredicate inPredicate)
	{
		int64_t attrFilteredValues = 0;
		int64_t readChunks = 0;
		int64_t readBlocks = 0;

		inPredicate->setEvaluateFunc(attrDesc->type_);
		auto inChunkItr = inArr->getChunkIterator(attrDesc->id_, iterateMode::EXIST);

		std::stringstream ss;
		while (!inChunkItr->isEnd())
		{
			if (inChunkItr->isExist())
			{
				//ss << "CHUNK [" << inChunkItr->seqPos() << "]: EXISt / ";
				auto inChunk = (**inChunkItr);
				auto outChunk = outArr->makeChunk(attrDesc->id_, inChunk->getId());
				//outChunk->bufferRef(inChunk);
				outChunk->bufferCopy(inChunk);

				int64_t chunkFilteredValue = 0;
				int64_t readChunkBlocks = 0;
				auto isEmptyChunk = this->chunkFilter<Ty_>(outChunk, inChunk, inPredicate, chunkFilteredValue, readChunkBlocks);
				if (isEmptyChunk)
				{
					outArr->freeChunk(attrDesc->id_, inChunk->getId());
					//BOOST_LOG_TRIVIAL(debug) << "[" << inChunk->getId() << "]: isEmpty";
				}

				attrFilteredValues += chunkFilteredValue;
				readBlocks += readChunkBlocks;
				readChunks++;
				//ss << "[" << inChunkItr->seqPos() << "]: true / ";
			} else
			{
				//ss << "CHUNK [" << inChunkItr->seqPos() << "]: NULL / ";
			}

			++(*inChunkItr);
		}

		//BOOST_LOG_TRIVIAL(debug) << ss.str();
		BOOST_LOG_TRIVIAL(debug) << "=====";
		BOOST_LOG_TRIVIAL(debug) << "Attr filtered value: " << attrFilteredValues;
		BOOST_LOG_TRIVIAL(debug) << "Chunks: " << readChunks;
		BOOST_LOG_TRIVIAL(debug) << "Blocks: " << readBlocks;
		BOOST_LOG_TRIVIAL(debug) << "=====";
	}

	template <class Ty_>
	bool chunkFilter(pChunk outChunk, pChunk inChunk, pPredicate inPredicate, int64_t& outFilteredValue, int64_t& outReadChunkBlocks)
	{
		bool isEmptyChunk = true;
		int64_t chunkFilteredValue = 0;
		int64_t readBlocks = 0;

		auto inBlockItr = inChunk->getBlockIterator();
		auto outBlockItr = outChunk->getBlockIterator();
		while (!inBlockItr->isEnd())
		{
			if (inBlockItr->isExist())
			{
				auto inBlock = (**inBlockItr);
				auto outBlock = outChunk->makeBlock(inBlock->getId());

				int64_t blockFilteredValue = 0;
				auto isEmptyBlock = this->blockFilter<Ty_>(outBlock, inBlock, inPredicate, blockFilteredValue);
				if (isEmptyBlock)
				{
					outChunk->freeBlock(inBlock->getId());
				} else
				{
					isEmptyChunk = false;
				}

				chunkFilteredValue += blockFilteredValue;
				++readBlocks;
			}

			++(*inBlockItr);
			++(*outBlockItr);
		}

		outFilteredValue = chunkFilteredValue;
		outReadChunkBlocks = readBlocks;
		return isEmptyChunk;
	}

	template <class Ty_>
	bool blockFilter(pBlock outBlock, pBlock inBlock, pPredicate inPredicate, int64_t& outFilteredValue)
	{
		int64_t filteredValue = 0;
		bool isEmpty = true;

		auto inBlockItemItr = inBlock->getItemIterator();
		auto outBlockItemItr = outBlock->getItemIterator();

		while (!inBlockItemItr->isEnd())
		{
			if (inBlockItemItr->isExist() && inPredicate->evaluate(inBlockItemItr))
			{
				outBlockItemItr->setExist();
				filteredValue++;
				isEmpty = false;
			} else
			{
				outBlockItemItr->setNull();
			}
			++(*inBlockItemItr);
			++(*outBlockItemItr);
		}

		outFilteredValue = filteredValue;

		return isEmpty;
	}
};
}		// core
}		// msdb
#endif	// _MSDB_INDEX_FILTER_ACTION_H_