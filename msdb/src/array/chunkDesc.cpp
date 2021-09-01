#include <pch.h>
#include <array/chunkDesc.h>

namespace msdb
{
namespace core
{
extern const chunkSize INVALID_CHUNK_SIZE = static_cast<chunkSize>(~0);

chunkDesc::chunkDesc()
{
	// TODO::initialization
}

chunkDesc::chunkDesc(const chunkId id,
					 pAttributeDesc attrDesc, const dimension& dims, const dimension& blockDims,
					 const coor sp, const coor ep, const chunkSize mSize)
	: id_(id), attrDesc_(attrDesc), dims_(dims), blockDims_(blockDims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(mSize),
	chunkCoor_(dims.size()), cType_(CompressionMethod::NONE), useCompression_(false)
{
	this->initChunkCoor();
	if (this->mSize_ == INVALID_CHUNK_SIZE)
	{
		this->initPhysicalChunkSizeFromDims();
	}
}

chunkDesc::chunkDesc(const chunkId id,
					 pAttributeDesc attrDesc, const dimension& dims, const dimension& blockDims,
					 const coor sp, const coor ep,
					 const chunkSize mSize, const chunkSize cSize,
					 const CompressionMethod cType)
	: id_(id), attrDesc_(attrDesc), dims_(dims), blockDims_(blockDims), sp_(sp), ep_(ep), mSize_(mSize), cSize_(cSize),
	chunkCoor_(dims.size()), cType_(cType), useCompression_(true)
{
	this->initChunkCoor();
	if(this->mSize_ == INVALID_CHUNK_SIZE)
	{
		this->initPhysicalChunkSizeFromDims();
	}
}

chunkDesc::chunkDesc(const chunkDesc& mit)
	: id_(mit.id_), attrDesc_(std::make_shared<attributeDesc>(*(mit.attrDesc_))),
	mSize_(mit.mSize_), cSize_(mit.cSize_), useCompression_(mit.useCompression_),
	cType_(mit.cType_), dims_(mit.dims_), blockDims_(mit.blockDims_), sp_(mit.sp_), ep_(mit.ep_), chunkCoor_(mit.chunkCoor_)
{
}

void chunkDesc::setDim(dimensionId dId, position_t value)
{
	this->dims_[dId] = value;
	this->initPhysicalChunkSizeFromDims();
}

dimension chunkDesc::getDims()
{
	return this->dims_;
}

dimension chunkDesc::getBlockDims()
{
	return this->blockDims_;
}
dimension chunkDesc::getBlockSpace()
{
	return this->dims_ / this->blockDims_;
}

size_t chunkDesc::getDimSize()
{
	return this->dims_.size();
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
}		// core
}		// msdb