#include <pch.h>
#include <array/chunkIterator.h>

namespace msdb
{
namespace core
{
chunkIterator::chunkIterator(const dimension dims,
							 chunkContainer* chunks, pBitmap chunkBitmap, 
							 iterateMode itMode)
	: mdItr(dims), 
	chunks_(chunks), chunkBitmap_(chunkBitmap),
	itMode_(itMode)
{
	if (itMode == iterateMode::EXIST)
	{
		if (!this->isExist(this->seqPos_))
		{
			++(*this);
		}
	}
}
chunkIterator::chunkIterator(const self_type& mit)
	: mdItr(mit), 
	chunks_(mit.chunks_), chunkBitmap_(mit.chunkBitmap_), 
	itMode_(mit.itMode_)
{
}

chunkIterator::size_type chunkIterator::getSeqEnd() const
{
	return this->chunks_->size();
}

bool chunkIterator::isExist() const
{
	return this->isExist(this->seqPos_);
}
bool chunkIterator::isExist()
{
	return this->isExist(this->seqPos_);
}

bool chunkIterator::isExist(const chunkId cid) const
{
	return (*this->chunkBitmap_)[cid] && this->chunks_ != nullptr && (this->chunks_->find(cid) != this->chunks_->end());
}
bool chunkIterator::needToMake() const
{
	return (*this->chunkBitmap_)[this->seqPos_] && (this->chunks_->find(this->seqPos_) == this->chunks_->end());
}

iterateMode chunkIterator::getIterateMode() const
{
	return this->itMode_;
}
const pBitmap chunkIterator::getChunkBitmap() const
{
	return this->chunkBitmap_;
}

//////////////////////////////
// ConstChunkIterator
constChunkIterator::constChunkIterator(const dimension dims,
	const chunkContainer* chunks, const pBitmap chunkBitmap,
	iterateMode itMode)
	: mdItr(dims),
	chunks_(chunks), chunkBitmap_(chunkBitmap),
	itMode_(itMode)
{
	if (itMode == iterateMode::EXIST)
	{
		if (!this->isExist(this->seqPos_))
		{
			++(*this);
		}
	}
}
constChunkIterator::constChunkIterator(const self_type& mit)
	: mdItr(mit),
	chunks_(mit.chunks_), chunkBitmap_(mit.chunkBitmap_),
	itMode_(mit.itMode_)
{
}

constChunkIterator::size_type constChunkIterator::getSeqEnd() const
{
	return this->chunks_->size();
}

bool constChunkIterator::isExist() const
{
	return this->isExist(this->seqPos_);
}

bool constChunkIterator::isExist(const chunkId cid) const
{
	return (*this->chunkBitmap_)[cid] && this->chunks_ != nullptr && (this->chunks_->find(cid) != this->chunks_->end());
}
bool constChunkIterator::needToMake() const
{
	return (*this->chunkBitmap_)[this->seqPos_] && (this->chunks_->find(this->seqPos_) == this->chunks_->end());
}

iterateMode constChunkIterator::getIterateMode() const
{
	return this->itMode_;
}
const pBitmap constChunkIterator::getChunkBitmap() const
{
	return this->chunkBitmap_;
}
}		// core
}		// msdb