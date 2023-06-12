#include <pch.h>
#include <array/blockIterator.h>

namespace msdb
{
namespace core
{
blockIterator::blockIterator(const dimension dims,
							 blockContainer* blocks, pBitmap blockBitmap, 
							 const iterateMode itMode)
	: mdItr(dims), 
	blocks_(blocks), blockBitmap_(blockBitmap),
	itMode_(itMode)
{
}

blockIterator::blockIterator(const self_type& mit)
	: mdItr(mit), 
	blocks_(mit.blocks_), blockBitmap_(mit.blockBitmap_),
	itMode_(mit.itMode_)
{
}

blockIterator::size_type blockIterator::getSeqEnd() const
{
	// TODO::getSeqEnd for blockIterator?
	return 0;
}

bool blockIterator::isExist() const
{
	if ((this->blocks_->size() > this->seqPos_) && (this->blocks_->at(this->seqPos_) != nullptr) && (this->blockBitmap_->isExist(this->seqPos_)))
		return true;

	return false;
}

bool blockIterator::isExist(const blockId cid) const
{
	return false;
}

iterateMode blockIterator::getIterateMode() const
{
	return this->itMode_;
}

//////////////////////////////
// constBlockIterator
constBlockIterator::constBlockIterator(const dimension dims,
	const blockContainer* blocks, const pBitmap blockBitmap,
	const iterateMode itMode)
	: mdItr(dims),
	blocks_(blocks), blockBitmap_(blockBitmap),
	itMode_(itMode)
{
}

constBlockIterator::constBlockIterator(const self_type& mit)
	: mdItr(mit),
	blocks_(mit.blocks_), blockBitmap_(mit.blockBitmap_),
	itMode_(mit.itMode_)
{
}

constBlockIterator::size_type constBlockIterator::getSeqEnd() const
{
	return 0;
}

bool constBlockIterator::isExist() const
{
	if ((this->blocks_->size() > this->seqPos_) && (this->blocks_->at(this->seqPos_) != nullptr) && (this->blockBitmap_->isExist(this->seqPos_)))
		return true;

	return false;
}

bool constBlockIterator::isExist(const blockId cid) const
{
	return false;
}

iterateMode constBlockIterator::getIterateMode() const
{
	return this->itMode_;
}

//////////////////////////////
// Single block iterator
singleBlockIterator::singleBlockIterator(const dimension dims, pBlock blk, const iterateMode itMode)
	: blockIterator(dims, nullptr, nullptr, itMode), block_(blk)
{
}

singleBlockIterator::singleBlockIterator(const singleBlockIterator& mit)
	: blockIterator(mit), block_(mit.block_)
{
}

blockIterator::size_type singleBlockIterator::getSeqEnd() const
{
	return 1;
}
bool singleBlockIterator::isExist() const
{
	if (this->seqPos_ == 0 && this->block_ != nullptr)
		return true;
	return false;
}

bool singleBlockIterator::isExist(const blockId bid) const
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

//////////////////////////////
// Const Single block iterator
constSingleBlockIterator::constSingleBlockIterator(const dimension dims, const pBlock blk, const iterateMode itMode)
	: constBlockIterator(dims, nullptr, nullptr, itMode), block_(blk)
{
}

constSingleBlockIterator::constSingleBlockIterator(const constSingleBlockIterator& mit)
	: constBlockIterator(mit), block_(mit.block_)
{
}

blockIterator::size_type constSingleBlockIterator::getSeqEnd() const
{
	return 1;
}
bool constSingleBlockIterator::isExist() const
{
	if (this->seqPos_ == 0 && this->block_ != nullptr)
		return true;
	return false;
}

bool constSingleBlockIterator::isExist(const blockId bid) const
{
	if (bid == 0 && this->block_ != nullptr)
		return true;
	return false;
}

void constSingleBlockIterator::next()
{
	if (this->seqPos_ == 0)
	{
		++this->seqPos_;
		this->end_ = true;
		this->front_ = false;
	}
}

void constSingleBlockIterator::prev()
{
	if (this->seqPos_ == 1)
	{
		--this->seqPos_;
		this->front_ = true;
		this->end_ = false;
	}
}
}		// core
}		// msdb