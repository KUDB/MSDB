﻿#pragma once
#ifndef _MSDB_SEHUFFMANCHUNK_H_
#define _MSDB_SEHUFFMANCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>
#include <compression/seChunk.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class seHuffmanChunk : public seChunk<Ty_>
{
public:
	seHuffmanChunk(pChunkDesc desc)
		: seChunk<Ty_>
	{

	}
	virtual ~seHuffmanChunk()
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
			auto blockObj = std::make_shared<seHuffmanBlock<Ty_>>(desc);
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
		bstream seOut;
		this->seEncode<Ty_>(seOut);

		out << setw(sizeof(size_type) * CHAR_BIT) << seOut.capacity();

		//////////////////////////////
		huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
		huffmanEncoder.encode(out, (uint8_t*)seOut.data(), seOut.capacity());
	}

	template<class Ty_>
	void deserializeTy(bstream& in)
	{
		size_type seSize = 0;
		in >> setw(sizeof(size_type) * CHAR_BIT) >> seSize;
		uint8_t* tempBuffer = new uint8_t[seSize];

		huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
		huffmanDecoder.decode((uint8_t*)tempBuffer, seSize, in);

		//////////////////////////////
		bstream seIn;
		seIn.resize(seSize);
		memcpy(seIn.data(), tempBuffer, seSize);
		delete[] tempBuffer;

		this->seDecode<Ty_>(seIn);
	}
};

//////////////////////////////
// seHuffmanChunkFactory
//
// To make concreteType of seHuffmanChunk
//
template <typename Ty_>
class seHuffmanChunkFactory : public chunkFactory
{
public:
	seHuffmanChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};		// seHuffmanChunkFactory

//////////////////////////////
// Factory constructor for seHuffmanChunkFacotry
//
class seHuffmanChunkFactoryBuilder
{
public:
	seHuffmanChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<seHuffmanChunkFactory<Ty_>>();
	}
};		// seHuffmanChunkFactoryBuilder

//////////////////////////////
// seHuffmanChunkType
//
class seHuffmanChunkType : public chunkType
{
public:
	seHuffmanChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "seHuffmanChunk";
	}
};		// seHuffmanChunkType
}		// core
}		// msdb

#include "seHuffmanChunk.hpp"

#endif	// _MSDB_SEHUFFMANCHUNK_H_