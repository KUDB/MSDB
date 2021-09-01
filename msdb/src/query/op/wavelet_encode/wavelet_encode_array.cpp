#include <pch.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <compression/wtChunk.h>

namespace msdb
{
namespace core
{
wavelet_encode_array::wavelet_encode_array(pArrayDesc desc)
	: base_type(desc), originalChunkDims_(desc->getDSize())
{
	//this->maxLevel_ = this->isMaxLevelAvailable(maxLevel);
	for(pDimensionDesc desc : *this->desc_->dimDescs_)
	{
		desc->chunkSize_ = desc->blockSize_;
	}
}

pChunk wavelet_encode_array::makeChunk(const attributeId attrId, const chunkId cId)
{
	auto desc = this->getChunkDesc(attrId, cId);
	auto chunkObj = std::make_shared<wtChunk>(desc);
	this->insertChunk(attrId, chunkObj);
	return chunkObj;
}

pChunk wavelet_encode_array::makeChunk(const chunkDesc& desc)
{
	auto chunkObj
		= std::make_shared<wtChunk>(std::make_shared<chunkDesc>(desc));
	this->insertChunk(desc.attrDesc_->id_, chunkObj);
	//chunkObj->setLevel(this->getMaxLevel());
	return chunkObj;
}

// TODO:: Change name to getWTLeve()
size_t wavelet_encode_array::getMaxLevel()
{
	return this->maxLevel_;
}

dimension wavelet_encode_array::getOrigianlChunkDims()
{
	return this->originalChunkDims_;
}

// TODO:: Change name to setWTLevel()
void wavelet_encode_array::setLevel(const size_t maxLevel)
{
	this->maxLevel_ = maxLevel;
}

void wavelet_encode_array::setOrigianlChunkDims(const dimension originalChunkSize)
{
	this->originalChunkDims_ = originalChunkSize;
}

bool wavelet_encode_array::isMaxLevelAvailable(const size_t maxLevel)
{
	size_t level = maxLevel;
	for (pDimensionDesc desc : *this->desc_->dimDescs_)
	{
		while(desc->chunkSize_ < pow(2, level) && level > 0)
		{
			--level;
		}
		if (level == 0)
		{
			return 0;
		}
	}

	return level;
}
}		// core
}		// msdb