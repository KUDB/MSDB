#pragma once
#ifndef _MSDB_OP_UPDATE_ACTION_H_
#define _MSDB_OP_UPDATE_ACTION_H_

#include <pch_op.h>
#include <query/opAction.h>
#include <util/enumType.h>
#include <io/file.h>
#include <io/io_util.h>

namespace msdb
{
namespace core
{
////////////////////////////////////////////////////////////
// Fast update action
////////////////////////////////////////////////////////////
// 
// - Fast update action is to make accelerate the update process.
// - The operator assumed that the order of the input data is maintained 
//   in an array 
// - Just copy the raw input data to the array buffer without considering 
//   any order.
//
class fast_update_action : public opAction
{
public:
	fast_update_action();
	~fast_update_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;

protected:
	size_t getBufferSize(const dimension inArrDim, const size_t typeSize);
	std::map<attributeId, std::string> getFilePathMapper(const pArray inArr);

private:
	template<typename Ty_>
	void updateFromFile(const concreteTy<Ty_>& type, pArray outArr, pArray inArr, pAttributeDesc attr, std::string& filePath, pQuery qry)
	{
		Ty_* fileData = nullptr;

		try
		{
			//size_t bufferSize = this->getBufferSize(inArr->getDesc()->getDimDescs()->getChunkDims(), sizeof(Ty_));
			auto chunkDim = inArr->getDesc()->getDimDescs()->getChunkDims();
			auto chunkDimArea = chunkDim.area();
			size_t bufferSize = chunkDimArea * attr->typeSize_;
			size_t fileLength = 0;

			if (isImageFile(filePath))
			{
				size_t numPixel = 0;
				readImageFile(filePath, (void**)(&fileData), fileLength, numPixel);
			}
			else
			{
				std::ifstream input(filePath, std::ios::binary);
				fileData = new Ty_[bufferSize / sizeof(Ty_)];

				if (!input)
					_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_CANNOT_OPEN_FILE));

				// Get file size
				std::streampos begin, end;
				begin = input.tellg();
				input.seekg(0, std::ios::end);
				end = input.tellg();
				fileLength = end - begin;

				// Init file pointer
				input.seekg(0, std::ios::beg);

				input.read((char*)(fileData), std::min(fileLength, bufferSize));
				input.close();
			}

			if (fileLength < bufferSize)
				BOOST_LOG_TRIVIAL(warning) << "File size(" << fileLength << ") is smaller than buffer size(" << bufferSize << ").";

			this->updateData(inArr, attr, fileData, std::min(fileLength, bufferSize));
		}
		catch(...)
		{
			BOOST_LOG_TRIVIAL(error) << "Error: updateFromFile(" << inArr->getDesc()->name_ << ", " << attr->getName();
		}

		if(fileData)
			delete[] fileData;
	}

	/**
	 * length: number of Ty_ elements
	 *
	 * Insert raw input to buffer without considering the order or coordinate of the data.
	 * Just use bufferCopy
	 */
	template<typename Ty_>
	void updateData(pArray inArr, pAttributeDesc attr, Ty_* data, size_t length)
	{
		auto dims = inArr->getDesc()->getDimDescs()->getDims();
		auto globalItr = mdItr(dims);

		auto startCoor = std::static_pointer_cast<coor>(this->params_[2]->getParam());
		auto endCoor = std::static_pointer_cast<coor>(this->params_[3]->getParam());

		try
		{
			inArr->itemCoorToChunkCoor(*startCoor);
			inArr->itemCoorToChunkCoor(*endCoor);

			// TODO::Need to range chunk iterator (startChunk -> endChunk);
			//       Implement 'setRange(start, end)' to default chunkIterator.
			//		 Remove all range Iterators and integrate the iterators to default iterators.

			// 주어진 범위 내의 데이터만 업데이트 하기 위해서는
			// (1) 주어진 범위의 chunk를 가져와야 함
			// (2) Chunk 내에서 올바른 item range 를 구해서 삽입해야함
			// 하지만 여기는 fastUpdate니까 Chunk만 지정해서 업데이트 하는 식으로 변경한다면 (1)만 만족해주면 될 듯
			auto chunkItr = inArr->getChunkIterator(attr->getId());
			chunkItr->moveTo(*startCoor);

			if (chunkItr->isEnd())
			{
				_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(
					MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT, 
					std::string("Out of Chunk Range. There is no Chunk for ") + startCoor->toString()));
			}

			if (!chunkItr->isExist())
			{
				inArr->makeChunk(attr->getId(), chunkItr->seqPos());
			}

			// 현재 코드에 다양한 문제가 발생할 수 있음
			// 데이터를 Load 하지 않고 업데이트 할 경우 chunk의 다른 내용이 모두 사라질 수 있음.
			// 수정된 영역만 표기해서 update를 진행해야 함.
			// 대신의 chunk를 이미지 하나씩으로 분리하면 현 단계에서 오류는 발생하지 않을 수 있음.
			
			pChunk myChunk = (**chunkItr);
			if (!myChunk->isMaterialized())
			{
				myChunk->bufferAlloc();
				myChunk->makeAllBlocks();
			}

			myChunk->bufferCopy((void*)data, std::min({ (size_t)(sizeof(Ty_) * length), (size_t)(myChunk->getBuffer()->size()) }));

			// TODO::Implement buffer Copy with Chunk Iteration
			// buffer Copy 하고 남은 것들은 계속 다음 chunk로 넘기면서 Copy 진행할 것
		}
		catch(...)
		{
			BOOST_LOG_TRIVIAL(error) << "Update: error for the attribute (" << attr->getId() << ")";
			throw;
		}
	}
};
}		// core 
}		// msdb
#endif	// _MSDB_OP_INSERT_ACTION_H_
