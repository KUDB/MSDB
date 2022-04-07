#pragma once
#ifndef _MSDB_ADAPTHUFFMANCHUNK_H_
#define _MSDB_ADAPTHUFFMANCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/huffmanBlock.h>
#include <compression/adaptiveHuffmanCode.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class adaptHuffmanChunk : public flattenChunk<Ty_>
{
public:
	adaptHuffmanChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}
	virtual ~adaptHuffmanChunk()
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
			this->serializeTy<uint64_t, uint16_t>(bs);
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
			this->deserializeTy<uint16_t, uint8_t>(bs);
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
		//////////////////////////////
		// Block based adaptive huffman coding
		//aHuffmanCoder<Cty_, Ty_> coder(sizeof(Ty_) * CHAR_BIT);
		//auto bit = this->getBlockIterator();
		//while(!bit->isEnd())
		//{
		//	if(bit->isExist())
		//	{
		//		pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
		//		hb->serializeTy<Cty_, Ty_>(coder, out);
		//	}
		//	++(*bit);
		//}

		//////////////////////////////
		// Adaptive huffman coding
		aHuffmanCoder<uint16_t, uint8_t> coder(sizeof(uint8_t) * CHAR_BIT);
		auto buffer = (const char*)this->getBuffer()->getReadData();
		auto size = this->getBuffer()->size();
		for (int i = 0; i < size; ++i)
		{
			coder.encode(out, buffer[i]);
		}

		//////////////////////////////
	}

	template<typename Cty_, typename Ty_>
	void deserializeTy(bstream& in)
	{
		//aHuffmanCoder<Cty_, Ty_> coder(sizeof(Ty_) * CHAR_BIT);
		//auto bit = this->getBlockIterator();
		//while (!bit->isEnd())
		//{
		//	if (bit->isExist())
		//	{
		//		auto a = *bit;
		//		pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
		//		hb->deserializeTy<Cty_, Ty_>(coder, in);
		//	}
		//	++(*bit);
		//}

		//////////////////////////////
		// Adaptive huffman coding
		aHuffmanCoder<uint16_t, uint8_t> coder(sizeof(uint8_t) * CHAR_BIT);
		auto buffer = (unsigned char*)this->getBuffer()->getData();
		auto size = this->getBuffer()->size();
		for (int i = 0; i < size; ++i)
		{
			unsigned char value = coder.decode(in);
			buffer[i] = value;
		}
	}
};

//////////////////////////////
// adaptHuffmanChunkFactory
//
// To make concreteType of adaptHuffmanChunk
//
template <typename Ty_>
class adaptHuffmanChunkFactory : public chunkFactory
{
public:
	adaptHuffmanChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};		// adaptHuffmanChunkFactory

//////////////////////////////
// Factory constructor for adaptHuffmanChunkFacotry
//
class adaptHuffmanChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	adaptHuffmanChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<adaptHuffmanChunkFactory<Ty_>>();
	}
};		// adaptHuffmanChunkFactoryBuilder

//////////////////////////////
// adaptHuffmanChunkType
//
class adaptHuffmanChunkType : public chunkType
{
public:
	adaptHuffmanChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "adaptHuffmanChunk";
	}
};		// adaptHuffmanChunkType
}		// core
}		// msdb

#include "adaptHuffmanChunk.hpp"

#endif	// _MSDB_ADAPTHUFFMANCHUNK_H_