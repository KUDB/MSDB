#include <pch.h>
#include <op/wavelet_decode/wavelet_decode_array.h>

namespace msdb
{
namespace core
{
wavelet_decode_array::wavelet_decode_array(pArrayDesc desc, const size_t maxLevel)
	: base_type(desc), maxLevel_(maxLevel)
{
	for(pDimensionDesc desc : *this->desc_->dimDescs_)
	{
		desc->chunkSize_ *= pow(2, this->maxLevel_);
	}
}
size_t wavelet_decode_array::getMaxLevel()
{
	return this->maxLevel_;
}
}		// core
}		// msdb