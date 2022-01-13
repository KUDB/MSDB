#include "insert_action.h"
#include "insert_plan.h"

namespace msdb
{
namespace core
{
template<typename Ty_>
void insert_action::insertFromFile(pArray inArr, pAttributeDesc attr)
{
	size_t bufferSize = this->getBufferSize(inArr->getDesc()->getDimDescs()->getDims(), sizeof(Ty_));
	Ty_* fileData = new Ty_[bufferSize];

	std::filesystem::path filePath = *(std::static_pointer_cast<std::string>(this->params_[2]->getParam()));
	std::ifstream in(filePath, std::ios::binary);

	// Get file size
	in.seekg(0, std::ios::end);
	size_t fileLength = (size_t)in.tellg();
	in.seekg(0, std::ios::beg);

	in.read((char*)(fileData), fileLength);
	in.close();

	this->insertData(inArr, fileData, fileLength);

	delete[] fileData;
}

//////////////////////////////
// TODO::Implement insert body "insertFromMemory" function
template <typename Ty_>
void insert_action::insertFromMemory(pArray inArr, pAttributeDesc attr)
{
	auto attrMemContainer = std::static_pointer_cast<insert_array_multi_attr_memory_pset::containerType>(this->params_[2]->getParam());
	// insert_array_multi_attr_memory_pset::valueType::paramType == opParamMemory
	auto memParam = attrMemContainer->at(attr->id_);
	auto tupleMem = std::static_pointer_cast<opParamMemory::paramType>(memParam.getParam());
	auto mem = std::get<0>(*tupleMem);
	auto memSize = std::get<1>(*tupleMem);

	size_t bufferSize = this->getBufferSize(inArr->getDesc()->getDimDescs()->getDims(), sizeof(Ty_));
	Ty_* fileData = new Ty_[bufferSize];

	//std::shared_ptr<void> mem = tupleAttrMemory->first;
	memcpy(fileData, mem.get(), memSize);

	// TODO:: copy memory to temporal buffer
	this->insertData(inArr, fileData, memSize / sizeof(Ty_));

	delete[] fileData;
}

template<typename Ty_>
void insert_action::insertData(pArray inArr, Ty_* data, size_t length)
{
	auto dims = inArr->getDesc()->getDimDescs()->getDims();
	auto globalItr = coorItr(dims);

	for (auto attr : *inArr->getDesc()->attrDescs_)
	{
		_MSDB_TRY_BEGIN
		{
			auto chunkItr = inArr->getChunkIterator(attr->id_);
			while (!chunkItr->isEnd())
			{
				inArr->makeChunk(attr->id_, chunkItr->seqPos());
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

								if (mySeqPos < length)
								{
									(**itemItr).set<Ty_>(data[mySeqPos]);
								}
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
			// TODO::logging exceptions
		}
		_MSDB_CATCH_END
	}
}
}		// core
}		// msdb