#pragma once
#ifndef _MSDB_HUFFMANCHUNK_H_
#define _MSDB_HUFFMANCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/huffmanBlock.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class huffmanChunk : public flattenChunk<Ty_>
{
public:
	huffmanChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}
	virtual ~huffmanChunk()
	{

	}

public:
	virtual pBlock makeBlock(const blockId bId) override
	{
		assert(this->blockCapacity_ > bId);
		if (this->blocks_[bId] == nullptr)
		{
			// Make new one
			auto desc = this->getBlockDesc(bId);
			auto blockObj = std::make_shared<huffmanBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
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
		case eleType::INT8:
		case eleType::UINT8:
			this->serializeTy<uint32_t, uint8_t>(bs);
			break;
		case eleType::INT16:
		case eleType::UINT16:
			this->serializeTy<uint32_t, uint16_t>(bs);
			break;
		case eleType::INT32:
		case eleType::UINT32:
			this->serializeTy<uint64_t, uint32_t>(bs);
			break;
		case eleType::INT64:
		case eleType::UINT64:
			//this->serializeTy<uint64_t, uint64_t>(bs);	// require uint128_t
			//break;
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
		case eleType::INT8:
		case eleType::UINT8:
			this->deserializeTy<uint32_t, uint8_t>(bs);
			break;
		case eleType::INT16:
		case eleType::UINT16:
			this->deserializeTy<uint32_t, uint16_t>(bs);
			break;
		case eleType::INT32:
		case eleType::UINT32:
			this->deserializeTy<uint64_t, uint32_t>(bs);
			break;
		case eleType::INT64:
		case eleType::UINT64:
		default:
			_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		}
	}

private:
	template<typename Cty_, typename Ty_>
	void serializeTy(bstream& out)
	{
		//huffmanCoderLonger<uint32_t, uint8_t> huffmanEncoder;
		huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
		huffmanEncoder.encode(out, (uint8_t*)this->getBuffer()->getReadData(), this->getBuffer()->size());
	}

	template<typename Cty_, typename Ty_>
	void deserializeTy(bstream& in)
	{
		//huffmanCoderLonger<uint32_t, uint8_t> huffmanDecoder;
		huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
		huffmanDecoder.decode((uint8_t*)this->getBuffer()->getData(), this->getBuffer()->size(), in);
	}
};

//////////////////////////////
// huffmanChunkFactory
//
// To make concreteType of huffmanChunk
//
template <typename Ty_>
class huffmanChunkFactory : public chunkFactory
{
public:
	huffmanChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};		// huffmanChunkFactory

//////////////////////////////
// Factory constructor for huffmanChunkFacotry
//
class huffmanChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	huffmanChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<huffmanChunkFactory<Ty_>>();
	}
};		// huffmanChunkFactoryBuilder

//////////////////////////////
// huffmanChunkType
//
class huffmanChunkType : public chunkType
{
public:
	huffmanChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "huffmanChunk";
	}
};		// huffmanChunkType
}		// core
}		// msdb

#include "huffmanChunk.hpp"

#endif	// _MSDB_HUFFMANCHUNK_H_