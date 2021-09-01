#pragma once
#ifndef _MSDB_OP_NAIVE_FILTER_ACTION_H_
#define _MSDB_OP_NAIVE_FILTER_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <parse/predicate.h>

namespace msdb
{
namespace core
{
class naive_filter_action : public opAction
{
public:
	naive_filter_action();
	virtual ~naive_filter_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery q);

private:
	template<class Ty_>
	void attributeFilter(pArray outArr, pArray inArr, pAttributeDesc attrDesc, pPredicate inPredicate)
	{
		int64_t attrFilteredValues = 0;

		auto inChunkItr = inArr->getChunkIterator();
		//auto outChunkItr = outArr->getChunkIterator();

		inPredicate->setEvaluateFunc(attrDesc->type_);

		while (!inChunkItr->isEnd()) 
		{
			if(inChunkItr->isExist())
			{
				auto inChunk = (**inChunkItr);
				auto outChunk = outArr->makeChunk(attrDesc->id_, inChunk->getId());
				outChunk->setChunkDesc(inChunk->getDesc());
				outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
				outChunk->bufferCopy(inChunk);
				//outChunk->bufferRef(inChunk);
				
				int64_t chunkFilteredValue = 0;
				auto isEmptyChunk = this->chunkFilter<Ty_>(outChunk, inChunk, inPredicate, chunkFilteredValue);
				if(isEmptyChunk)
				{
					outArr->freeChunk(inChunk->getId());
				}

				attrFilteredValues += chunkFilteredValue;
			}
			++(*inChunkItr);
			//++(*outChunkItr);
		}

		BOOST_LOG_TRIVIAL(debug) << "=====";
		BOOST_LOG_TRIVIAL(debug) << "Attr filtered value: " << attrFilteredValues;
		BOOST_LOG_TRIVIAL(debug) << "=====";
	}

	template <class Ty_>
	bool chunkFilter(pChunk outChunk, pChunk inChunk, pPredicate inPredicate, int64_t& outFilteredValue)
	{
		bool isEmptyChunk = true;
		int64_t chunkFilteredValue = 0;

		auto inBlockItr = inChunk->getBlockIterator();
		auto outBlockItr = outChunk->getBlockIterator();
		while (!inBlockItr->isEnd())
		{
			if(inBlockItr->isExist())
			{
				auto inBlock = (**inBlockItr);
				auto outBlock = outChunk->makeBlock(inBlock->getId());
				
				outBlock->setBlockDesc(inBlock->getDesc());
				outBlock->copyBitmap(inBlock->getBitmap());

				int64_t blockFilteredValue = 0;
				auto isEmptyBlock = this->blockFilter<Ty_>(outBlock, inBlock, inPredicate, blockFilteredValue);
				if(isEmptyBlock)
				{
					outChunk->freeBlock(inBlock->getId());
				}else
				{
					isEmptyChunk = false;
				}

				chunkFilteredValue += blockFilteredValue;
				++(*inBlockItr);
				++(*outBlockItr);
			}
		}

		outFilteredValue = chunkFilteredValue;
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
				//std::cout << "setExist: "
				//	<< static_cast<int>((**outBlockItemItr).get<char>()) << std::endl;
				outBlockItemItr->setExist();
				filteredValue++;
				isEmpty = false;
			}else
			{
				//std::cout << "setNull: "
				//	<< static_cast<int>((**outBlockItemItr).get<char>()) << std::endl;
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
#endif // _MSDB_OP_NAIVE_FILTER_ACTION_H_
