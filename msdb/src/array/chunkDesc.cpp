#include <pch.h>
#include <array/chunkDesc.h>

namespace msdb
{
namespace core
{
extern const chunkSize INVALID_CHUNK_SIZE = static_cast<chunkSize>(~0);

chunkDesc::chunkDesc()
	: id_(0), attrDesc_(nullptr), mSize_(0), cSize_(0), useCompression_(false), cType_(encodingType::NONE),
	dims_(1), blockDims_(1), sp_(1), ep_(1), chunkCoor_(1)
{
}

chunkDesc::chunkDesc(const chunkId id,
					 pAttributeDesc attrDesc, const dimension& dims, const dimension& blockDims,
					 const coor& sp, const coor& ep, const chunkSize mSize)
	: id_(id), attrDesc_(attrDesc), dims_(dims), blockDims_(blockDims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(mSize),
	chunkCoor_(dims.size()), cType_(encodingType::NONE), useCompression_(false)
{
	this->initChunkCoor();
	if (this->mSize_ == INVALID_CHUNK_SIZE)
	{
		this->initPhysicalChunkSizeFromDims();
	}
}

//chunkDesc::chunkDesc(const chunkId id,
//					 pAttributeDesc attrDesc, const dimension& dims, const dimension& blockDims,
//					 const coor sp, const coor ep,
//					 const chunkSize mSize, const chunkSize cSize,
//					 const CompressionMethod cType)
//	: id_(id), attrDesc_(attrDesc), dims_(dims), blockDims_(blockDims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(cSize),
//	chunkCoor_(dims.size()), cType_(cType), useCompression_(true)
//{
//	this->initChunkCoor();
//	if(this->mSize_ == INVALID_CHUNK_SIZE)
//	{
//		this->initPhysicalChunkSizeFromDims();
//	}
//}

chunkDesc::chunkDesc(const chunkDesc& mit)
	: id_(mit.id_), attrDesc_(std::make_shared<attributeDesc>(*(mit.attrDesc_))),
	mSize_(mit.mSize_), cSize_(mit.cSize_), useCompression_(mit.useCompression_),
	cType_(mit.cType_), dims_(mit.dims_), blockDims_(mit.blockDims_), sp_(mit.sp_), ep_(mit.ep_), chunkCoor_(mit.chunkCoor_)
{
}

chunkDesc::chunkDesc(chunkDesc&& src) noexcept
	: chunkDesc()
{
	swap(*this, src);
}

void chunkDesc::setDim(dimensionId dId, position_t value)
{
	this->dims_[dId] = value;
	this->initPhysicalChunkSizeFromDims();
}

dimension chunkDesc::getDims() const
{
	return this->dims_;
}

dimension chunkDesc::getBlockDims() const
{
	return this->blockDims_;
}
dimension chunkDesc::getBlockSpace() const
{
	return this->dims_ / this->blockDims_;
}

size_t chunkDesc::getDimSize() const
{
	return this->dims_.size();
}

pAttributeDesc chunkDesc::getAttrDesc() const
{
	return this->attrDesc_;
}

//////////////////////////////
// Operators
// ***************************
// Assign
chunkDesc& chunkDesc::operator=(const chunkDesc& src)
{
	if (this == &src)
	{
		return *this;
	}

	chunkDesc temp(src);
	swap(*this, temp);
	return *this;
}
chunkDesc& chunkDesc::operator=(chunkDesc&& src) noexcept
{
	chunkDesc temp(std::move(src));
	swap(*this, temp);
	return *this;
}

void chunkDesc::initPhysicalChunkSizeFromDims()
{
	this->mSize_ = this->attrDesc_->typeSize_;
	for(dimensionId d = 0; d < this->dims_.size(); d++)
	{
		this->mSize_ *= this->dims_[d];
	}

	if(this->cSize_ != INVALID_CHUNK_SIZE && this->cSize_ > this->mSize_)
	{
		this->cSize_ = this->mSize_;
	}
}

void chunkDesc::initChunkCoor()
{
	for(dimensionId d = 0; d < this->dims_.size(); d++)
	{
		// this->ep_[d] - this->sp_[d] = chunk width
		this->chunkCoor_[d] = this->sp_[d] / (this->ep_[d] - this->sp_[d]);
	}
}

// ***************************
// Friends
void swap(chunkDesc& first, chunkDesc& second) noexcept
{
	using std::swap;

	swap(first.id_, second.id_);
	swap(first.attrDesc_, second.attrDesc_);

	swap(first.mSize_, second.mSize_);
	swap(first.cSize_, second.cSize_);

	swap(first.useCompression_, second.useCompression_);
	swap(first.cType_, second.cType_);

	swap(first.dims_, second.dims_);
	swap(first.blockDims_, second.blockDims_);
	swap(first.sp_, second.sp_);
	swap(first.ep_, second.ep_);
	swap(first.chunkCoor_, second.chunkCoor_);
}
bool operator==(const chunkDesc& lhs_, const chunkDesc& rhs_)
{
	if (lhs_.id_ != rhs_.id_)	return false;

	// shared_ptr obj
	if (lhs_.attrDesc_ != rhs_.attrDesc_)
	{
		if (lhs_.attrDesc_ == nullptr || rhs_.attrDesc_ == nullptr)	return false;
		if (*(lhs_.attrDesc_) != *(rhs_.attrDesc_))					return false;
	}

	if (lhs_.mSize_ != rhs_.mSize_)	return false;
	if (lhs_.cSize_ != rhs_.cSize_)	return false;

	if (lhs_.useCompression_ != rhs_.useCompression_)	return false;
	if (lhs_.cType_ != rhs_.cType_)	return false;

	if (lhs_.dims_ != rhs_.dims_)	return false;
	if (lhs_.blockDims_ != rhs_.blockDims_)	return false;
	if (lhs_.sp_ != rhs_.sp_)	return false;
	if (lhs_.ep_ != rhs_.ep_)	return false;
	if (lhs_.chunkCoor_ != rhs_.chunkCoor_)	return false;

	return true;
}
bool operator!=(const chunkDesc& lhs_, const chunkDesc& rhs_)
{
	if (lhs_ == rhs_)
		return false;

	return true;
}
}		// core
}		// msdb