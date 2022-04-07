﻿#include <pch.h>
#include <compression/compassChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// compassChunkType
//
compassChunkType::compassChunkType(const dataType& type)
	: chunkType(buildFactory(compassChunkFactoryBuilder(), type))
{
}

//REGISTER_CHUNK_FACTORY("compassChunk", compassChunkFactoryBuilder())

//compassChunk::compassChunk(pChunkDesc desc)
//	: flattenChunk<element>(desc)
//{
//}
//
//compassChunk::~compassChunk()
//{
//}
//
//pBlock compassChunk::makeBlock(const blockId bId)
//{
//	assert(this->blockCapacity_ > bId);
//	if (this->blocks_[bId] == nullptr)
//	{
//		auto desc = this->getBlockDesc(bId);
//		auto blockObj = std::make_shared<compassBlock<Ty_>>(desc);
//		this->insertBlock(blockObj);
//		return blockObj;
//	}
//	return this->blocks_[bId];
//}
//void compassChunk::serialize(std::ostream& os)
//{
//	bstream bs;
//	switch (this->desc_->attrDesc_->type_)
//	{
//	case eleType::CHAR:
//		this->serializeTy<char>(bs);
//		break;
//	case eleType::INT8:
//		this->serializeTy<int8_t>(bs);
//		break;
//	case eleType::INT16:
//		this->serializeTy<int16_t>(bs);
//		break;
//	case eleType::INT32:
//		this->serializeTy<int32_t>(bs);
//		break;
//	case eleType::INT64:
//		this->serializeTy<int64_t>(bs);
//		break;
//	case eleType::UINT8:
//		this->serializeTy<uint8_t>(bs);
//		break;
//	case eleType::UINT16:
//		this->serializeTy<uint16_t>(bs);
//		break;
//	case eleType::UINT32:
//		this->serializeTy<uint32_t>(bs);
//		break;
//	case eleType::UINT64:
//		this->serializeTy<uint64_t>(bs);
//		break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//
//	this->serializedSize_ = bs.capacity();
//	this->getOutHeader()->serialize(os);
//	os.write(bs.data(), bs.capacity());
//}
//void compassChunk::deserialize(std::istream& is)
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
//		this->deserializeTy<char>(bs);
//		break;
//	case eleType::INT8:
//		this->deserializeTy<int8_t>(bs);
//		break;
//	case eleType::INT16:
//		this->deserializeTy<int16_t>(bs);
//		break;
//	case eleType::INT32:
//		this->deserializeTy<int32_t>(bs);
//		break;
//	case eleType::INT64:
//		this->deserializeTy<int64_t>(bs);
//		break;
//	case eleType::UINT8:
//		this->deserializeTy<uint8_t>(bs);
//		break;
//	case eleType::UINT16:
//		this->deserializeTy<uint16_t>(bs);
//		break;
//	case eleType::UINT32:
//		this->deserializeTy<uint32_t>(bs);
//		break;
//	case eleType::UINT64:
//		this->deserializeTy<uint64_t>(bs);
//		break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//}
//void compassChunk::setNumBins(size_t numBins)
//{
//	this->numBins_ = numBins;
//}
}		// core
}		// msdb
