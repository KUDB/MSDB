#pragma once
#ifndef _MSDB_SPIHTCHUNK_H_
#define _MSDB_SPIHTCHUNK_H_

#include <pch.h>
#include <array/monoChunk.h>
#include <compression/spihtBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
//class spihtChunk;
//using pSpihtChunk = std::shared_ptr<spihtChunk>;

template <typename Ty_>
class spihtChunk : public monoChunk<Ty_>
{
public:
	spihtChunk(pChunkDesc desc)
		: monoChunk<Ty_>(desc)
	{
	}
	virtual ~spihtChunk()
	{

	}

	void initBufferZero()
	{
		memset(this->cached_->getData(), 0, this->desc_->mSize_);
	}

protected:
	virtual void makeBuffer()
	{
		this->cached_ = std::make_shared<monoChunkBuffer>();
	}

public:
	virtual pBlock makeBlock(const blockId bId) override
	{
		auto desc = this->getBlockDesc(bId);
		desc->mSize_ = desc->mSize_;
		desc->mOffset_ = 0;
		auto blockObj = std::make_shared<spihtBlock<Ty_>>(desc);
		blockObj->setLevel(this->maxLevel_);
		this->insertBlock(blockObj);
		return blockObj;
	}

public:
	//virtual pChunkItemIterator getItemIterator();
	//virtual pChunkItemRangeIterator getItemRangeIterator(const range& range);

public:
	virtual void serialize(std::ostream& os) override
	{
		bstream bs;
		switch (this->desc_->attrDesc_->type_)
		{
		case eleType::CHAR:
			this->serialize<char>(bs);
			break;
		case eleType::INT8:
			this->serialize<int8_t>(bs);
			break;
		case eleType::INT16:
			this->serialize<int16_t>(bs);
			break;
		case eleType::INT32:
			this->serialize<int32_t>(bs);
			break;
		case eleType::INT64:
			this->serialize<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->serialize<uint8_t>(bs);
			break;
		case eleType::UINT16:
			this->serialize<uint16_t>(bs);
			break;
		case eleType::UINT32:
			this->serialize<uint32_t>(bs);
			break;
		case eleType::UINT64:
			this->serialize<uint64_t>(bs);
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
			this->deserialize<char>(bs);
			break;
		case eleType::INT8:
			this->deserialize<int8_t>(bs);
			break;
		case eleType::INT16:
			this->deserialize<int16_t>(bs);
			break;
		case eleType::INT32:
			this->deserialize<int32_t>(bs);
			break;
		case eleType::INT64:
			this->deserialize<int64_t>(bs);
			break;
		case eleType::UINT8:
			this->deserialize<uint8_t>(bs);
			break;
		case eleType::UINT16:
			this->deserialize<uint16_t>(bs);
			break;
		case eleType::UINT32:
			this->deserialize<uint32_t>(bs);
			break;
		case eleType::UINT64:
			this->deserialize<uint64_t>(bs);
			break;
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			// TODO::check isExist()
			std::static_pointer_cast<spihtBlock>(**blockItr)->setLevel(this->maxLevel_);
			this->blockSerialize<Ty_>(bs, (**blockItr));
			++(*blockItr);
		}
	}

	template <typename Ty_>
	void blockSerialize(bstream& bs, pBlock curBlock)
	{
		pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(curBlock);
		spBlock->serializeTy<Ty_>(bs);
	}

	template<class Ty_>
	void deserialize(bstream& bs)
	{
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if(blockItr->isExist())
			{
				std::static_pointer_cast<spihtBlock>(**blockItr)->setLevel(this->maxLevel_);
				this->blockDeserialize<Ty_>(bs, (**blockItr));
			}
			
			++(*blockItr);
		}
	}

	template <typename Ty_>
	void blockDeserialize(bstream& bs, pBlock curBlock)
	{
		pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(curBlock);
		spBlock->deserializeTy<Ty_>(bs);
	}

public:
	inline void setLevel(size_t maxLevel)
	{
		this->maxLevel_ = maxLevel;
	}
	inline size_t getLevel()
	{
		return this->maxLevel_;
	}

protected:
	size_t maxLevel_;
};
}		// core
}		// msdb
#endif	// _MSDB_SPIHTCHUNK_H_