﻿#pragma once
#ifndef _MSDB_COMPASSCHUNK_H_
#define _MSDB_COMPASSCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/compassBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
//class compassChunk;
//using pCompassChunk = std::shared_ptr<compassChunk>;

template <typename Ty_>
class compassChunk : public flattenChunk<Ty_>
{
public:
	compassChunk(pChunkDesc desc)
		:flattenChunk<Ty_>(desc)
	{

	}
	virtual ~compassChunk()
	{

	}

public:
	virtual pBlock makeBlock(const blockId bId) override
	{
		assert(this->blockCapacity_ > bId);
		if (this->blocks_[bId] == nullptr)
		{
			auto desc = this->getBlockDesc(bId);
			auto blockObj = std::make_shared<compassBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
			return blockObj;
		}
		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override
	{
		bstream bs;
		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->serializeTy<char>(bs);
			break;
		case eleType::INT8:
			this->serializeTy<int8_t>(bs);
			break;
		case eleType::INT16:
			this->serializeTy<int16_t>(bs);
			break;
		case eleType::INT32:
			this->serializeTy<int32_t>(bs);
			break;
		case eleType::INT64:
			this->serializeTy<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->serializeTy<uint8_t>(bs);
			break;
		case eleType::UINT16:
			this->serializeTy<uint16_t>(bs);
			break;
		case eleType::UINT32:
			this->serializeTy<uint32_t>(bs);
			break;
		case eleType::UINT64:
			this->serializeTy<uint64_t>(bs);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}

		this->serializedSize_ = bs.capacity();
		this->getOutHeader()->serialize(os);
		os.write(bs.data(), bs.capacity());
	}
	virtual void deserialize(std::istream& is) override
	{
		this->getHeader()->deserialize(is);
		this->updateFromHeader();

		bstream bs;
		bs.resize(this->serializedSize_);
		is.read(bs.data(), this->serializedSize_);

		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->deserializeTy<char>(bs);
			break;
		case eleType::INT8:
			this->deserializeTy<int8_t>(bs);
			break;
		case eleType::INT16:
			this->deserializeTy<int16_t>(bs);
			break;
		case eleType::INT32:
			this->deserializeTy<int32_t>(bs);
			break;
		case eleType::INT64:
			this->deserializeTy<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->deserializeTy<uint8_t>(bs);
			break;
		case eleType::UINT16:
			this->deserializeTy<uint16_t>(bs);
			break;
		case eleType::UINT32:
			this->deserializeTy<uint32_t>(bs);
			break;
		case eleType::UINT64:
			this->deserializeTy<uint64_t>(bs);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	template<typename Ty_>
	void serializeTy(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
				cpBlock->setNumBins(this->numBins_);
				try
				{
					cpBlock->serializeTy<Ty_>(bs);
				} catch (...)
				{
					std::cout << "Exception" << std::endl;
				}
			}

			++(*blockItr);
		}
	}

	template<class Ty_>
	void deserializeTy(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				pCompassBlock cpBlock = std::static_pointer_cast<compassBlock>(**blockItr);
				cpBlock->setNumBins(this->numBins_);
				cpBlock->deserializeTy<Ty_>(bs);
			}

			++(*blockItr);
		}
	}

public:
	inline void setNumBins(size_t numBins)
	{
		this->numBins_ = numBins;
	}

private:
	size_t numBins_;
};
}	// core
}	// msdb
#endif	// _MSDB_COMPASSCHUNK_H_
