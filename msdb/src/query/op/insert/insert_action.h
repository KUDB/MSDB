#pragma once
#ifndef _MSDB_OP_INSERT_ACTION_H_
#define _MSDB_OP_INSERT_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class insert_action : public opAction
{
public:
	insert_action();
	~insert_action();
	
public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

private:
	template<typename Ty_>
	void inserting(std::vector<pArray>& inputArrays)
	{
		Ty_* data;
		pArray inArr = inputArrays[0];
		auto chunkItr = inArr->getChunkIterator();
		while (!chunkItr->isEnd())
		{
			if (chunkItr->isExist())
			{
				auto inChunk = (**chunkItr);
				auto chunkRange = inChunk->getChunkRange();

				//auto outChunk = outArr->makeChunk(attr->id_, inChunk->getId());
				//outChunk->setChunkDesc(inChunk->getDesc());
				//outChunk->copyBlockBitmap(inChunk->getBlockBitmap());
				//outChunk->bufferCopy(inChunk);

				auto blockItr = inChunk->getBlockIterator();
				while (!blockItr->isEnd())
				{
					if (blockItr->isExist())
					{
						auto inBlock = (**blockItr);
						//auto outBlock = outChunk->makeBlock(inBlock->getId());

						auto itemItr = inBlock->getItemIterator();
						size_t dSize = inBlock->getDSize();
						dimension blockDims = inBlock->getDesc()->dims_;

						size_t block_num = 1;
						for (int d = (int)dSize - 1; d >= 0; d--)
						{
							block_num *= blockDims[d];
						}

						for (size_t i = 0; i < block_num; i++)
						{
							// TODO ##########################
							size_t pos = itemItr->seqPos();
							(**itemItr).set<Ty_>(data[pos]);

							++(*itemItr);
						}				
					}

					++(*blockItr);
				}
			}

			++(*chunkItr);
		}
	}

};
}		// core
}		// msdb
#endif // _MSDB_OP_INSERT_ACTION_H_
