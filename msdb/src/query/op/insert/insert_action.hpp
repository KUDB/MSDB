﻿#include "insert_action.h"
#include "insert_plan.h"

namespace msdb
{
namespace core
{
template<typename Ty_>
void insert_action::insertFromFile(pArray inArr, pAttributeDesc attr)
{
	size_t bufferSize = this->getBufferSize(inArr->getDesc()->getDimDescs()->getDims(), sizeof(Ty_));
	Ty_* fileData = new Ty_[bufferSize / sizeof(Ty_)];

	_MSDB_TRY_BEGIN
	{
		std::filesystem::path filePath = *(std::static_pointer_cast<std::string>(this->params_[2]->getParam()));
		std::ifstream in(filePath, std::ios::binary);

		if (!in)
		{
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_CANNOT_OPEN_FILE));
		}
		// Get file size
		//in.seekg(0, std::ios::end);
		std::streampos begin, end;
		begin = in.tellg();
		in.seekg(0, std::ios::end);
		end = in.tellg();

		size_t fileLength = end - begin;
		in.seekg(0, std::ios::beg);

		if (fileLength < bufferSize)
		{
			BOOST_LOG_TRIVIAL(warning) << "File size(" << fileLength << ") is smaller than buffer size(" << bufferSize << ").";
		}

		in.read((char*)(fileData), std::min(fileLength, bufferSize));
		in.close();

		this->insertData(inArr, attr, fileData, std::min(fileLength, bufferSize));
	}
	_MSDB_CATCH_ALL
	{
		BOOST_LOG_TRIVIAL(error) << "Error: insertFromFile(" << inArr->getDesc()->name_ << ", " << attr->getName();
	}

	delete[] fileData;
}

template <typename Ty_>
void insert_action::insertFromMemory(pArray inArr, pAttributeDesc attr)
{
	size_t bufferSize = this->getBufferSize(inArr->getDesc()->getDimDescs()->getDims(), sizeof(Ty_));
	Ty_* fileData = new Ty_[bufferSize];

	_MSDB_TRY_BEGIN
	{
		auto attrMemContainer = std::static_pointer_cast<insert_array_multi_attr_memory_pset::containerType>(this->params_[2]->getParam());

		auto memParam = attrMemContainer->at(attr->id_);
		auto tupleMem = std::static_pointer_cast<opParamMemory::paramType>(memParam.getParam());
		auto mem = std::get<0>(*tupleMem);
		auto memSize = std::get<1>(*tupleMem);

		//std::shared_ptr<void> mem = tupleAttrMemory->first;
		memcpy(fileData, mem.get(), memSize);

		// TODO:: copy memory to temporal buffer
		this->insertData(inArr, attr, fileData, memSize / sizeof(Ty_));
	}
	_MSDB_CATCH(const std::out_of_range & e)
	{
		BOOST_LOG_TRIVIAL(error) << "Insert: There is no data (from memory) for the attribute (" << attr->id_ << ")";
		throw;
	}
	_MSDB_CATCH_ALL
	{
		BOOST_LOG_TRIVIAL(error) << "Insert: error for the attribute (" << attr->id_ << ")";
		throw;
	}
	_MSDB_CATCH_END

	delete[] fileData;
}

template<typename Ty_>
void insert_action::insertData(pArray inArr, pAttributeDesc attr, Ty_* data, size_t length)
{
	auto dims = inArr->getDesc()->getDimDescs()->getDims();
	auto globalItr = mdItr(dims);

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
		BOOST_LOG_TRIVIAL(error) << "Insert: error for the attribute (" << attr->id_ << ")";
		throw;
	}
	_MSDB_CATCH_END
}
}		// core
}		// msdb