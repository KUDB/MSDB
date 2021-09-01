#include <pch.h>
#include <compression/lzwChunk.h>
#include <util/ioutil.h>

namespace msdb
{
namespace core
{
lzwChunk::lzwChunk(pChunkDesc desc)
	: memBlockChunk(desc)
{
}
lzwChunk::~lzwChunk()
{
}
pBlock lzwChunk::makeBlock(const blockId bId)
{
	assert(this->blockCapacity_ > bId);
	if (this->blocks_[bId] == nullptr)
	{
		// Make new one
		auto desc = this->getBlockDesc(bId);
		auto blockObj = std::make_shared<lzwBlock>(desc);
		this->insertBlock(blockObj);
	}

	return this->blocks_[bId];
}

void lzwChunk::serialize(std::ostream& os)
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
//void lzwChunk::serialize(std::ostream& os)
//{
//	std::stringstream oss;
//	switch (this->desc_->attrDesc_->type_)
//	{
//	case eleType::CHAR:
//		this->serializeTy<char>(oss);
//		break;
//	case eleType::INT8:
//		this->serializeTy<int8_t>(oss);
//		break;
//	case eleType::INT16:
//		this->serializeTy<int16_t>(oss);
//		break;
//	case eleType::INT32:
//		this->serializeTy<int32_t>(oss);
//		break;
//	case eleType::INT64:
//		this->serializeTy<int64_t>(oss);
//		break;
//	case eleType::UINT8:
//		this->serializeTy<uint8_t>(oss);
//		break;
//	case eleType::UINT16:
//		this->serializeTy<uint16_t>(oss);
//		break;
//	case eleType::UINT32:
//		this->serializeTy<uint32_t>(oss);
//		break;
//	case eleType::UINT64:
//		this->serializeTy<uint64_t>(oss);
//		break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//
//	this->serializedSize_ = getSize(oss);
//	this->getOutHeader()->serialize(os);
//	os << oss.str();
//}

void lzwChunk::deserialize(std::istream& is)
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
//void lzwChunk::deserialize(std::istream& is)
//{
//	this->getHeader()->deserialize(is);
//	this->updateFromHeader();
//
//	std::stringstream iss;
//	iss << is.rdbuf();
//
//	switch (this->desc_->attrDesc_->type_)
//	{
//	case eleType::CHAR:
//		this->deserializeTy<char>(iss);
//		break;
//	case eleType::INT8:
//		this->deserializeTy<int8_t>(iss);
//		break;
//	case eleType::INT16:
//		this->deserializeTy<int16_t>(iss);
//		break;
//	case eleType::INT32:
//		this->deserializeTy<int32_t>(iss);
//		break;
//	case eleType::INT64:
//		this->deserializeTy<int64_t>(iss);
//		break;
//	case eleType::UINT8:
//		this->deserializeTy<uint8_t>(iss);
//		break;
//	case eleType::UINT16:
//		this->deserializeTy<uint16_t>(iss);
//		break;
//	case eleType::UINT32:
//		this->deserializeTy<uint32_t>(iss);
//		break;
//	case eleType::UINT64:
//		this->deserializeTy<uint64_t>(iss);
//		break;
//	default:
//		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
//	}
//}
}		// core
}		// msdb