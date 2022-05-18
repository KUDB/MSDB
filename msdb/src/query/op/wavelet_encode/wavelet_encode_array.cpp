#include <pch.h>
#include <op/wavelet_encode/wavelet_encode_array.h>
#include <op/wavelet_encode/wtChunk.h>

namespace msdb
{
namespace core
{
wavelet_encode_array::wavelet_encode_array(pArrayDesc desc)
	: base_type(desc), originalChunkDims_(desc->getDSize()), maxLevel_(0)
{
	initChunkFactories();

	//this->maxLevel_ = this->isMaxLevelAvailable(maxLevel);
	for(pDimensionDesc desc : *this->desc_->dimDescs_)
	{
		desc->chunkSize_ = desc->blockSize_;
	}
}

wavelet_encode_array::~wavelet_encode_array()
{
}

// TODO::Erase deprecated codes
//pChunk wavelet_encode_array::makeChunk(const attributeId attrId, const chunkId cId)
//{
//	// TODO::Do with chunkFactory
//	auto desc = this->getChunkDesc(attrId, cId);
//	auto chunkObj = std::make_shared<wtChunk>(desc);
//	this->insertChunk(chunkObj);
//	return chunkObj;
//}
//
//pChunk wavelet_encode_array::makeChunk(const chunkDesc& desc)
//{
//	// TODO::Do with chunkFactory
//	auto chunkObj
//		= std::make_shared<wtChunk>(std::make_shared<chunkDesc>(desc));
//	this->insertChunk(chunkObj);
//	//chunkObj->setLevel(this->getMaxLevel());
//	return chunkObj;
//}

// TODO:: Change name to getWTLevel()
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