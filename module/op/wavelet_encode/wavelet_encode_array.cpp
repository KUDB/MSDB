#include <pch_op.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <op/wavelet_encode/wtChunk.h>

namespace msdb
{
namespace core
{
wavelet_encode_array::wavelet_encode_array(pArrayDesc desc)
	: base_type(desc), originalChunkDims_(desc->getDSize())
{
	initChunkFactories();

	for(pDimensionDesc desc : *this->desc_->dimDescs_)
	{
		desc->chunkSize_ = desc->blockSize_;
	}
}

wavelet_encode_array::~wavelet_encode_array()
{
}

dimension wavelet_encode_array::getOrigianlChunkDims()
{
	return this->originalChunkDims_;
}

void wavelet_encode_array::setOrigianlChunkDims(const dimension originalChunkSize)
{
	this->originalChunkDims_ = originalChunkSize;
}

void wavelet_encode_array::initChunkFactories()
{
	wtChunkFactoryBuilder fb;

	this->cFactories_.clear();

	auto desc = this->getDesc()->getAttrDescs();
	for (auto attrDesc : *desc)
	{
		assert(attrDesc->id_ == this->cFactories_.size());

		this->cFactories_.push_back(std::visit(fb, attrDesc->dataType_));
	}
}
}		// core
}		// msdb