#include <pch.h>
#include <op/se_compression/seChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// seChunkType
//
seChunkType::seChunkType(const dataType& type)
	: chunkType(buildFactory(seChunkFactoryBuilder(), type))
{
}

//REGISTER_CHUNK_FACTORY("seChunk", seChunkFactoryBuilder())

//seChunk::seChunk(pChunkDesc desc)
//	: flattenChunk<element>(desc), level_(0), rBitFromMMT(0), min_(0)
//{
//}
//
//inline seChunk::~seChunk()
//{
//}
//
//size_t seChunk::getLevel()
//{
//	return this->level_;
//}
////chunkId seChunk::getSourceChunkId()
////{
////	return this->sourceChunkId_;
////}
////void seChunk::setSourceChunkId(chunkId cid)
////{
////	this->sourceChunkId_ = cid;
////}
//
//void seChunk::setLevel(size_t level)
//{
//	this->level_ = level;
//}


//void seChunk::serialize(std::ostream& os)
//{
//	bstream bs;
//	switch (this->desc_->attrDesc_->type_)
//	{
//	case eleType::CHAR:
//		this->serialize<char>(bs);
//		break;
//	case eleType::INT8:
//		this->serialize<int8_t>(bs);
//		break;
//	case eleType::INT16:
//		this->serialize<int16_t>(bs);
//		break;
//	case eleType::INT32:
//		this->serialize<int32_t>(bs);
//		break;
//	case eleType::INT64:
//		this->serialize<int64_t>(bs);
//		break;
//	case eleType::UINT8:
//		this->serialize<int8_t>(bs);
//		break;
//	case eleType::UINT16:
//		this->serialize<int16_t>(bs);
//		break;
//	case eleType::UINT32:
//		this->serialize<int32_t>(bs);
//		break;
//	case eleType::UINT64:
//		this->serialize<int64_t>(bs);
//		break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//
//	this->serializedSize_ = bs.capacity();
//	this->getOutHeader()->serialize(os);
//	os.write(bs.data(), bs.capacity());
//}
//
//void seChunk::deserialize(std::istream& is)
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
//		this->deserialize<char>(bs);
//		break;
//	case eleType::INT8:
//		this->deserialize<int8_t>(bs);
//		break;
//	case eleType::INT16:
//		this->deserialize<int16_t>(bs);
//		break;
//	case eleType::INT32:
//		this->deserialize<int32_t>(bs);
//		break;
//	case eleType::INT64:
//		this->deserialize<int64_t>(bs);
//		break;
//	case eleType::UINT8:
//		this->deserialize<int8_t>(bs);
//		break;
//	case eleType::UINT16:
//		this->deserialize<int16_t>(bs);
//		break;
//	case eleType::UINT32:
//		this->deserialize<int32_t>(bs);
//		break;
//	case eleType::UINT64:
//		this->deserialize<int64_t>(bs);
//		break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//}
//void seChunk::serializeGap(bstream& bs, int64_t gap)
//{
//	if (gap < 0)
//	{
//		bs << setw(1) << 0x1;
//		//BOOST_LOG_TRIVIAL(debug) << "GAP < 0";
//	} else
//	{
//		bs << setw(1) << 0x0;
//	}
//	if (gap)
//	{
//		assert(abs_(gap) <= 128);
//		bs << setw(abs_(gap));
//		bs << (uint64_t)0;
//	}
//	bs << setw(1) << 0x1;
//}
//int64_t seChunk::deserializeGap(bstream& bs)
//{
//	bs >> setw(1);
//	char negiveGap;
//	bs >> negiveGap;
//
//	size_t gap = 0;
//	char gapBit = 0;
//	do
//	{
//		assert(gap <= 128);
//		bs >> gapBit;
//		++gap;
//	} while (gapBit != 1);
//
//	if (negiveGap)
//	{
//		return (gap - 1) * -1;
//	}
//	return gap - 1;
//}
//void seChunk::setTileOffset(std::vector<uint64_t>& offset)
//{
//	this->tileOffset_ = offset;
//}
}		// core
}		// msdb