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
	void inserting(pArray inArr)
	{
		auto dims = inArr->getDesc()->getDimDescs()->getDims();
		size_t capacity = 1;
		for (int i = 0; i < dims.size(); ++i)
		{
			capacity *= dims[i];
		}
		size_t bufferSize = sizeof(Ty_) * capacity;

		Ty_* fileData = new Ty_[bufferSize];
		auto globalItr = coorItr(dims);

		_MSDB_TRY_BEGIN
		{
			std::filesystem::path filePath = *(std::static_pointer_cast<std::string>(this->params_[1]->getParam()));
			std::ifstream in(filePath, std::ios::binary);

			// Get file size
			in.seekg(0, std::ios::end);
			size_t fileLength = (size_t)in.tellg();
			in.seekg(0, std::ios::beg);

			in.read((char*)(fileData), fileLength);
			in.close();

			auto attrDesc = inArr->getDesc()->attrDescs_->at(0);

			auto chunkItr = inArr->getChunkIterator();
			while (!chunkItr->isEnd())
			{
				inArr->makeChunk(attrDesc->id_, chunkItr->seqPos());
				if (chunkItr->isExist())
				{
					auto inChunk = (**chunkItr);
					inChunk->bufferAlloc();
					inChunk->makeAllBlocks();
					auto chunkGlobalCoor = inChunk->getDesc()->sp_;
					auto blockItr = inChunk->getBlockIterator();
					while (!blockItr->isEnd())
					{
						if (blockItr->isExist())
						{
							auto inBlock = (**blockItr);
							auto itemItr = inBlock->getItemIterator();
							auto blockGlobalCoor = chunkGlobalCoor + inBlock->getDesc()->getSp();

							while (!itemItr->isEnd())
							{
								auto mySeqPos = globalItr.coorToSeq(itemItr->coor() + blockGlobalCoor);

								(**itemItr).set<Ty_>(fileData[mySeqPos]);
								++(*itemItr);
							}
						}

						++(*blockItr);
					}
				}

				++(*chunkItr);
			}
		}
		_MSDB_CATCH_ALL
		{

		}
		_MSDB_CATCH_END

		delete[] fileData;
	}
};
}		// core
}		// msdb
#endif // _MSDB_OP_INSERT_ACTION_H_
