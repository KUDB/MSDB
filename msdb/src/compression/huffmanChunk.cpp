﻿#include <pch.h>
#include <op/huffman_encode/huffmanChunk.h>
#include <op/huffman_encode/huffmanBlock.h>
#include <compression/adaptiveHuffmanCode.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// huffmanChunkType
//
huffmanChunkType::huffmanChunkType(const dataType& type)
	: chunkType(buildFactory(huffmanChunkFactoryBuilder(), type))
{
}

//REGISTER_CHUNK_FACTORY("huffmanChunk", huffmanChunkFactoryBuilder())

//huffmanChunk::huffmanChunk(pChunkDesc desc)
//	: flattenChunk<element>(desc)
//{
//}
//
//huffmanChunk::~huffmanChunk()
//{
//}
//
//pBlock huffmanChunk::makeBlock(const blockId bId)
//{
//	assert(this->blockCapacity_ > bId);
//	if (this->blocks_[bId] == nullptr)
//	{
//		// Make new one
//		auto desc = this->getBlockDesc(bId);
//		auto blockObj = std::make_shared<huffmanBlock>(desc);
//		this->insertBlock(blockObj);
//	}
//
//	return this->blocks_[bId];
//}
//
//void huffmanChunk::serialize(std::ostream& os)
//{
//	bstream bs;
//	switch (this->desc_->attrDesc_->type_)
//	{
//	case eleType::CHAR:
//	case eleType::INT8:
//	case eleType::UINT8:
//		this->serializeTy<uint32_t, uint8_t>(bs);
//		break;
//	case eleType::INT16:
//	case eleType::UINT16:
//		this->serializeTy<uint32_t, uint16_t>(bs);
//		break;
//	case eleType::INT32:
//	case eleType::UINT32:
//		this->serializeTy<uint64_t, uint32_t>(bs);
//		break;
//	case eleType::INT64:
//	case eleType::UINT64:
//		//this->serializeTy<uint64_t, uint64_t>(bs);	// require uint128_t
//		//break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	} 
//
//	this->serializedSize_ = bs.capacity();
//	this->getOutHeader()->serialize(os);
//	os.write(bs.data(), bs.capacity());
//}
//
//void huffmanChunk::deserialize(std::istream& is)
//{
//	this->getHeader()->deserialize(is);
//	this->updateFromHeader();
//
//	bstream bs;
//	bs.resize(this->serializedSize_);
//	is.read(bs.data(), this->serializedSize_);
//
//	switch (this->desc_->attrDesc_->type_)
//	{
//	case eleType::CHAR:
//	case eleType::INT8:
//	case eleType::UINT8:
//		this->deserializeTy<uint32_t, uint8_t>(bs);
//		break;
//	case eleType::INT16:
//	case eleType::UINT16:
//		this->deserializeTy<uint32_t, uint16_t>(bs);
//		break;
//	case eleType::INT32:
//	case eleType::UINT32:
//		this->deserializeTy<uint64_t, uint32_t>(bs);
//		break;
//	case eleType::INT64:
//	case eleType::UINT64:
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//}
}	// core
}	// msdb