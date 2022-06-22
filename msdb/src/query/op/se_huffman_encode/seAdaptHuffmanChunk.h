#pragma once
#ifndef _MSDB_SEADAPTHUFFMANCHUNK_H_
#define _MSDB_SEADAPTHUFFMANCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>
#include <op/se_compression/seChunk.h>
#include <compression/huffmanCode.h>
#include "seHuffmanBlock.h"

namespace msdb
{
namespace core
{
template <typename Ty_>
class seAdaptHuffmanChunk : public seChunk<Ty_>
{
public:
	using lzwCodeType = std::uint16_t;

public:
	seAdaptHuffmanChunk(pChunkDesc desc)
		: seChunk<Ty_>(desc)
	{
	}

	virtual ~seAdaptHuffmanChunk()
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
			auto blockObj = std::make_shared<seHuffmanBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
		}

		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override
	{
		bstream bs;
		bstream seOut;
		this->seEncode(seOut);
		//seChunk<Ty_>::seEncode(seOut);

		bs << setw(sizeof(size_t) * CHAR_BIT) << seOut.capacity();

		//////////////////////////////
		
		huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
		huffmanEncoder.encode(bs, (uint8_t*)seOut.data(), seOut.capacity());
		//////////////////////////////
		// 
		//switch (this->desc_->attrDesc_->type_)
		//{
		//case eleType::CHAR:
		//	this->serializeTy<char>(bs);
		//	break;
		//case eleType::INT8:
		//	this->serializeTy<int8_t>(bs);
		//	break;
		//case eleType::INT16:
		//	this->serializeTy<int16_t>(bs);
		//	break;
		//case eleType::INT32:
		//	this->serializeTy<int32_t>(bs);
		//	break;
		//case eleType::INT64:
		//	this->serializeTy<int64_t>(bs);
		//	break;
		//case eleType::UINT8:
		//	this->serializeTy<uint8_t>(bs);
		//	break;
		//case eleType::UINT16:
		//	this->serializeTy<uint16_t>(bs);
		//	break;
		//case eleType::UINT32:
		//	this->serializeTy<uint32_t>(bs);
		//	break;
		//case eleType::UINT64:
		//	this->serializeTy<uint64_t>(bs);
		//	break;
		//default:
		//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		//}

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

		size_t seSize = 0;
		bs >> setw(sizeof(size_t) * CHAR_BIT) >> seSize;
		uint8_t* tempBuffer = new uint8_t[seSize];

		huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
		huffmanDecoder.decode((uint8_t*)tempBuffer, seSize, bs);

		//////////////////////////////
		bstream seIn;
		seIn.resize(seSize);
		memcpy(seIn.data(), tempBuffer, seSize);
		delete[] tempBuffer;

		this->seDecode(seIn);
		//////////////////////////////

		//switch (this->desc_->attrDesc_->type_)
		//{
		//case eleType::CHAR:
		//	this->deserializeTy<char>(bs);
		//	break;
		//case eleType::INT8:
		//	this->deserializeTy<int8_t>(bs);
		//	break;
		//case eleType::INT16:
		//	this->deserializeTy<int16_t>(bs);
		//	break;
		//case eleType::INT32:
		//	this->deserializeTy<int32_t>(bs);
		//	break;
		//case eleType::INT64:
		//	this->deserializeTy<int64_t>(bs);
		//	break;
		//case eleType::UINT8:
		//	this->deserializeTy<uint8_t>(bs);
		//	break;
		//case eleType::UINT16:
		//	this->deserializeTy<uint16_t>(bs);
		//	break;
		//case eleType::UINT32:
		//	this->deserializeTy<uint32_t>(bs);
		//	break;
		//case eleType::UINT64:
		//	this->deserializeTy<uint64_t>(bs);
		//	break;
		//default:
		//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
		//}
	}

	//template<typename Ty_>
	//void serializeTy(bstream& out)
	//{
	//	bstream seOut;
	//	this->seEncode<Ty_>(seOut);

	//	out << setw(sizeof(size_t) * CHAR_BIT) << seOut.capacity();

	//	//////////////////////////////
	//	huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
	//	huffmanEncoder.encode(out, (uint8_t*)seOut.data(), seOut.capacity());
	//}

	//template<class Ty_>
	//void deserializeTy(bstream& in)
	//{
	//	size_t seSize = 0;
	//	in >> setw(sizeof(size_t) * CHAR_BIT) >> seSize;
	//	uint8_t* tempBuffer = new uint8_t[seSize];

	//	huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
	//	huffmanDecoder.decode((uint8_t*)tempBuffer, seSize, in);

	//	//////////////////////////////
	//	bstream seIn;
	//	seIn.resize(seSize);
	//	memcpy(seIn.data(), tempBuffer, seSize);
	//	delete[] tempBuffer;

	//	this->seDecode<Ty_>(seIn);
	//}
};

template <>
class seAdaptHuffmanChunk<double> : public seChunk<double>
{
public:
	seAdaptHuffmanChunk(pChunkDesc desc)
		: seChunk<double>(desc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "seAdaptHuffmanChunk: double"));
	}
};

template <>
class seAdaptHuffmanChunk<float> : public seChunk<float>
{
public:
	seAdaptHuffmanChunk(pChunkDesc desc)
		: seChunk<float>(desc)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_NOT_IMPLEMENTED, "seAdaptHuffmanChunk: float"));
	}
};

//////////////////////////////
// seAdaptHuffmanChunkFactory
//
// To make concreteType of seAdaptHuffmanChunk
//
template <typename Ty_>
class seAdaptHuffmanChunkFactory : public chunkFactory
{
public:
	seAdaptHuffmanChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};		// seAdaptHuffmanChunkFactory

//////////////////////////////
// Factory constructor for seAdaptHuffmanChunkFacotry
//
class seAdaptHuffmanChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	seAdaptHuffmanChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<seAdaptHuffmanChunkFactory<Ty_>>();
	}
};		// seAdaptHuffmanChunkFactoryBuilder

//////////////////////////////
// seAdaptHuffmanChunkType
//
class seAdaptHuffmanChunkType : public chunkType
{
public:
	seAdaptHuffmanChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "seAdaptHuffmanChunk";
	}
};		// seAdaptHuffmanChunkType
}		// core
}		// msdb

#include "seAdaptHuffmanChunk.hpp"

#endif	// _MSDB_SEADAPTHUFFMANCHUNK_H_