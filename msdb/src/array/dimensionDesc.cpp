#include <pch.h>
#include <array/dimensionDesc.h>
#include <util/math.h>

namespace msdb
{
namespace core
{
dimensionDesc::dimensionDesc(dimensionId id, std::string name,
							 dimension_type start, dimension_type end, 
							 position_t chunkSize, position_t blockSize)
	: id_(id), name_(name), start_(start), end_(end), 
	chunkSize_(chunkSize), blockSize_(blockSize)
{
}

dimensionDesc::dimensionDesc(const dimensionDesc& mit)
	: id_(mit.id_), name_(mit.name_), start_(mit.start_), end_(mit.end_),
	chunkSize_(mit.chunkSize_), blockSize_(mit.blockSize_)
{
}

position_t msdb::core::dimensionDesc::getLength()
{
	return this->end_ - this->start_;
}
size_t dimensionDesc::getChunkNum()
{
	if(this->getLength() % this->chunkSize_)
	{
		return this->getLength() / this->chunkSize_ + 1;
	}
	return this->getLength() / this->chunkSize_;
}
dimensionDescs::dimensionDescs()
{
}
dimensionDescs::dimensionDescs(const dimensionDescs& mit)
{
	for(auto desc : mit)
	{
		this->push_back(std::make_shared<dimensionDesc>(*desc));
	}
}
dimension dimensionDescs::getDims()
{
	dimension dims(this->size());
	for (dimensionId d = 0; d < this->size(); ++d)
	{
		dims[d] = this->at(d)->getLength();
	}
	return dims;
}
dimension dimensionDescs::getChunkDims()
{
	dimension dims(this->size());
	for (dimensionId d = 0; d < this->size(); ++d)
	{
		dims[d] = this->at(d)->chunkSize_;
	}
	return dims;
}

dimension dimensionDescs::getChunkSpace()
{
	return this->getDims() / this->getChunkDims();
}

dimension dimensionDescs::getBlockDims()
{
	dimension blockDims(this->size());
	for (dimensionId d = 0; d < this->size(); ++d)
	{
		blockDims[d] = this->at(d)->blockSize_;
	}
	return blockDims;
}

dimension dimensionDescs::getBlockSpace()
{
	return dimension(this->getChunkDims()) / this->getBlockDims();
}
}		// core
}		// msdb