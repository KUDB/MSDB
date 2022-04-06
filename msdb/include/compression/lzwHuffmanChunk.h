#pragma once
#ifndef _MSDB_LZWHUFFMANCHUNK_H_
#define _MSDB_LZWHUFFMANCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>
#include <compression/lzwEncoder.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class lzwHuffmanChunk : public flattenChunk<Ty_>
{
public:
	lzwHuffmanChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}
	virtual ~lzwHuffmanChunk()
	{

	}

	using lzwCodeType = std::uint16_t;

public:
	virtual pBlock makeBlock(const blockId bId) override
	{
		assert(this->blockCapacity_ > bId);
		if (this->blocks_[bId] == nullptr)
		{
			// Make new one
			auto desc = this->getBlockDesc(bId);
			auto blockObj = std::make_shared<lzwHuffmanBlock<Ty_>(desc);
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
	void serializeTy(bstream& out)
	{
		bstream lzwOut;
		lzwCoder<lzwCodeType> lzwEncoder;
		lzwEncoder.encode(lzwOut, (const unsigned char*)this->cached_->getReadData(), this->cached_->size());

		out << setw(sizeof(size_type) * CHAR_BIT) << lzwOut.capacity();

		//////////////////////////////
		huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
		huffmanEncoder.encode(out, (uint8_t*)lzwOut.data(), lzwOut.capacity());
	}

	template<class Ty_>
	void deserializeTy(bstream& in)
	{
		size_type lzwSize = 0;
		in >> setw(sizeof(size_type) * CHAR_BIT) >> lzwSize;

		uint8_t* tempBuffer = new uint8_t[lzwSize];

		huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
		huffmanDecoder.decode((uint8_t*)tempBuffer, lzwSize, in);

		bstream lzwIn;
		lzwIn.resize(lzwSize);
		memcpy(lzwIn.data(), tempBuffer, lzwSize);

		delete[] tempBuffer;

		lzwCoder<lzwCodeType> lzwDecoder;
		lzwDecoder.decode((unsigned char*)this->cached_->getData(), this->cached_->size(), lzwIn);
	}

private:
	static const lzwCodeType dms{ std::numeric_limits<lzwCodeType>::max() };
};

//////////////////////////////
// lzwHuffmanChunkFactory
//
// To make concreteType of lzwHuffmanChunk
//
template <typename Ty_>
class lzwHuffmanChunkFactory : public chunkFactory
{
public:
	lzwHuffmanChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for lzwHuffmanChunkFacotry
//
class lzwHuffmanChunkFactoryBuilder
{
public:
	lzwHuffmanChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<lzwHuffmanChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// lzwHuffmanChunkType
//
class lzwHuffmanChunkType : public chunkType
{
public:
	lzwHuffmanChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "lzwHuffmanChunk";
	}
};
}		// core
}		// msdb

#include "lzwHuffmanChunk.hpp"

#endif	// _MSDB_LZWHUFFMANCHUNK_H_