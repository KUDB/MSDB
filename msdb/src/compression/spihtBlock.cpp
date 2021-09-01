#include <pch.h>
#include <compression/spihtBlock.h>

namespace msdb
{
namespace core
{
spihtBlock::spihtBlock(pBlockDesc desc)
	: memBlock(desc)
{
}

spihtBlock::~spihtBlock()
{
}

void spihtBlock::init(dimension& bandDims)
{
	size_t dSize = this->getDSize();
	dimension blockDims = this->desc_->dims_;

	this->LIP_.clear();
	this->LIS_.clear();
	this->LIS_TYPE_.clear();
	this->LSP_.clear();

	size_t index = 0;
	size_t band_num = 1;
	for (int d = (int)dSize - 1; d >= 0; d--)
	{
		band_num *= bandDims[d];
	}
	size_t noChild_num = band_num / 4;

	for (size_t i = 0; i < band_num; i++)
	{
		this->LIP_.push_back(index);

		if (index >= noChild_num)
		{
			this->LIS_.push_back(index);
			this->LIS_TYPE_.push_back('A');
		}

		index++;
	}
}

void spihtBlock::setLevel(size_t maxLevel)
{
	this->maxLevel_ = maxLevel;
}
}		// core
}		// msdb