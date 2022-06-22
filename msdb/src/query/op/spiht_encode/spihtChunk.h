#pragma once
#ifndef _MSDB_SPIHTCHUNK_H_
#define _MSDB_SPIHTCHUNK_H_

#include <pch.h>
#include <array/monoChunk.h>
#include <op/spiht_encode/spihtBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
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
		// TODO::serialize spihtChunk
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if (blockItr->isExist())
			{
				auto sBlock = std::static_pointer_cast<spihtBlock<Ty_>>(**blockItr);
				sBlock->setLevel(this->maxLevel_);
				sBlock->serializeTy<Ty_>(bs);
				//this->blockSerialize<Ty_>(bs, (**blockItr));
			}

			++(*blockItr);
		}
	}

	//template <typename Ty_>
	//void blockSerialize(bstream& bs, pBlock curBlock)
	//{
	//	// TODO::serialize spihtChunk
	//	pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(curBlock);
	//	spBlock->serializeTy<Ty_>(bs);
	//}

	template<class Ty_>
	void deserialize(bstream& bs)
	{
		// TODO::deserialize spihtChunk
		auto blockItr = this->getBlockIterator();
		while (!blockItr->isEnd())
		{
			if(blockItr->isExist())
			{
				auto sBlock = std::static_pointer_cast<spihtBlock<Ty_>>(**blockItr);
				sBlock->setLevel(this->maxLevel_);
				sBlock->deserializeTy<Ty_>(bs);
				//this->blockDeserialize<Ty_>(bs, (**blockItr));
				
			}
			
			++(*blockItr);
		}
	}

	//template <typename Ty_>
	//void blockDeserialize(bstream& bs, pBlock curBlock)
	//{
	//	// TODO::deserialize spihtChunk
	//	pSpihtBlock spBlock = std::static_pointer_cast<spihtBlock>(curBlock);
	//	spBlock->deserializeTy<Ty_>(bs);
	//}

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

//////////////////////////////
// spihtChunkFactory
//
// To make concreteType of spihtChunk
//
template <typename Ty_>
class spihtChunkFactory : public chunkFactory
{
public:
	spihtChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for spihtChunkFacotry
//
class spihtChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	spihtChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<spihtChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// spihtChunkType
//
class spihtChunkType : public chunkType
{
public:
	spihtChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "spihtChunk";
	}
};
}		// core
}		// msdb

#include "spihtChunk.hpp"

#endif	// _MSDB_SPIHTCHUNK_H_