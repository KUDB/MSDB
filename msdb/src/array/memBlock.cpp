#include <pch.h>
#include <array/memBlock.h>
#include <array/memBlockItemIterator.h>

namespace msdb
{
namespace core
{
memBlock::memBlock(pBlockDesc desc)
	: block(desc)
{
	if(this->desc_->mSize_ == INVALID_BLOCK_SIZE || 
	   this->desc_->mOffset_ == INVALID_BLOCK_SIZE)
	{
		this->desc_->mSize_
			= this->desc_->dims_.area() * getEleSize(this->desc_->eType_);
		this->desc_->mOffset_ = this->desc_->mSize_ * this->desc_->id_;
	}
}

memBlock::~memBlock()
{
	this->desc_ = nullptr;
}

void memBlock::serialize(bstream& bs)
{
	switch (this->desc_->eType_)
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
}

void memBlock::deserialize(bstream& bs)
{
	switch (this->desc_->eType_)
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

pBlockItemIterator memBlock::getItemIterator()
{
	//return std::make_shared<memBlockItemIterator>(this->cached_->getData(),
	//											  this->desc_->eType_,
	//											  this->desc_->dims_,
	//											  this->desc_->sp_,
	//											  this->itemBitmap_);
	return std::make_shared<memBlockItemIterator>(this->cached_->getData(),
													   this->desc_->eType_,
													   this->desc_->dims_,
													   coorRange(this->desc_->getIsp(), this->desc_->getIep()),
													   this->desc_->getSp(),
													   this->itemBitmap_);
}

pBlockItemRangeIterator memBlock::getItemRangeIterator(const coorRange& range)
{
	auto sp = getOutsideCoor(this->desc_->getIsp(), range.getSp());
	auto ep = getInsideCoor(this->desc_->getIep(), range.getEp());
	return std::make_shared<memBlockItemRangeIterator>(this->cached_->getData(),
													   this->desc_->eType_,
													   this->desc_->dims_,
													   coorRange(sp, ep),
													   this->desc_->getSp(),
													   this->itemBitmap_);
}
void memBlock::refChunkBufferWithoutOwnership(void* data, bufferSize size)
{
	this->cached_ = std::make_shared<memBlockBuffer>(data, size);	// TODO::make mem block buffer
}
}		// core
}		// msdb