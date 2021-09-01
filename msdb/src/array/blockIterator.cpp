#include <pch.h>
#include <array/blockIterator.h>

namespace msdb
{
namespace core
{
blockIterator::blockIterator(const dimension dims,
							 blockContainer* blocks, pBitmap blockBitmap, 
							 const iterateMode itMode)
	: coorItr(dims), 
	blocks_(blocks), blockBitmap_(blockBitmap),
	itMode_(itMode)
{
}

blockIterator::blockIterator(const self_type& mit)
	: coorItr(mit), 
	blocks_(mit.blocks_), blockBitmap_(mit.blockBitmap_),
	itMode_(mit.itMode_)
{
}

blockIterator::size_type blockIterator::getSeqEnd()
{
	return 0;
}

bool blockIterator::isExist()
{
	if ((this->blocks_->size() > this->seqPos_) && (this->blocks_->at(this->seqPos_) != nullptr) && (this->blockBitmap_->isExist(this->seqPos_)))
		return true;

	return false;
}

bool blockIterator::isExist(blockId cid)
{
	return false;
}

iterateMode blockIterator::getIterateMode()
{
	return this->itMode_;
}

singleBlockIterator::singleBlockIterator(const dimension dims, pBlock blk, const iterateMode itMode)
	: blockIterator(dims, nullptr, nullptr, itMode), block_(blk)
{
}

singleBlockIterator::singleBlockIterator(const singleBlockIterator& mit)
	: blockIterator(mit), block_(mit.block_)
{
}

blockIterator::size_type singleBlockIterator::getSeqEnd()
{
	return 1;
}

bool singleBlockIterator::isExist()
{
	if (this->seqPos_ == 0 && this->block_ != nullptr)
		return true;
	return false;
}

bool singleBlockIterator::isExist(blockId bid)
{
	if (bid == 0 && this->block_ != nullptr)
		return true;
	return false;
}

void singleBlockIterator::next()
{
	if(this->seqPos_ == 0)
	{
		++this->seqPos_;
		this->end_ = true;
		this->front_ = false;
	}
}

void singleBlockIterator::prev()
{
	if(this->seqPos_ == 1)
	{
		--this->seqPos_;
		this->front_ = true;
		this->end_ = false;
	}
}
}		// core
}		// msdb