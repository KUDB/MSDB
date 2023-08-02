#include <pch.h>
#include <array/block.h>

namespace msdb
{
namespace core
{
block::block(pBlockDesc desc)
	: desc_(desc), itemBitmap_(std::make_shared<bitmap>(desc->dims_.area(), true))
{

}
block::~block()
{
	this->cached_ = nullptr;
	this->itemBitmap_ = nullptr;
}
blockId block::getId() const
{
	return this->desc_->id_;
}
pBlockDesc block::getDesc()
{
	return this->desc_;
}
const pBlockDesc block::getDesc() const
{
	return this->desc_;
}
dimensionId block::getDSize() const
{
	return this->desc_->dims_.size();
}
range block::getBlockRange() const
{
	return range(this->desc_->getSp(), this->desc_->getEp());
}
range block::getBlockItemRange() const
{
	return range(this->desc_->getIsp(), this->desc_->getIep());
}
void block::setBlockDesc(const pBlockDesc inDesc)
{
	*this->desc_ = *inDesc;
}
void block::unlinkFromChunkBuffer()
{
	this->cached_ = nullptr;
}
void block::copy(pBlock srcBlock)
{
	assert(srcBlock->getDesc()->mSize_ <= this->getDesc()->mSize_);
	assert(this->isMaterialized() == true);

	this->cached_->copy(srcBlock->getBuffer()->getData(), srcBlock->getDesc()->mSize_);
}
bool block::isMaterialized() const
{
	if (this->cached_ == nullptr)
	{
		return false;
	}

	return true;
}
pBlockBuffer block::getBuffer()
{
	return this->cached_;
}
// TODO::Block comparison
bool block::operator==(const block& rhs) const
{
	if (*(this->getDesc()) != *rhs.getDesc())	return false;

	auto lit = this->getItemIterator();
	auto rit = this->getItemIterator();

	if (lit->getCapacity() != rit->getCapacity())	return false;
	if (lit->isEnd() != rit->isEnd())	return false;

	while (lit->isEnd())
	{
		// TODO::Important::Block comparison

		++(*lit);
		++(*rit);
	}
}
void block::copyBitmap(cpBitmap itemBitmap)
{
	this->itemBitmap_ = std::make_shared<bitmap>(*itemBitmap);
}
void block::replaceBitmap(pBitmap itemBitmap)
{
	this->itemBitmap_ = itemBitmap;
}
void block::mergeBitmap(pBitmap itemBitmap)
{
	this->itemBitmap_->andMerge(*itemBitmap);
}
void block::initEmptyBitmap()
{
	this->itemBitmap_ = std::make_shared<bitmap>(this->desc_->dims_.area(), false);
}
pBitmap block::getBitmap()
{
	return this->itemBitmap_;
}
void block::print()
{
	switch (this->desc_->eType_)
	{
	case eleType::BOOL:
		return this->printImp<bool>();
	case eleType::CHAR:
		return this->printImp<char>();
	case eleType::INT8:
		return this->printImp<int8_t>();
	case eleType::INT16:
		return this->printImp<int16_t>();
	case eleType::INT32:
		return this->printImp<int32_t>();
	case eleType::INT64:
		return this->printImp<int64_t>();
	case eleType::UINT8:
		return this->printImp<uint8_t>();
	case eleType::UINT16:
		return this->printImp<uint16_t>();
	case eleType::UINT32:
		return this->printImp<uint32_t>();
	case eleType::UINT64:
		return this->printImp<uint64_t>();
	case eleType::FLOAT:
		return this->printImp<float>();
	case eleType::DOUBLE:
		return this->printImp<double>();
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
}
void* block::getDataPtr() const
{
	if (this->isMaterialized())
	{
		return this->cached_->getData();
	}
	return nullptr;
}
}		// core
}		// msdb