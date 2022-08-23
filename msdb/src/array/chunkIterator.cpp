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
}

//chunkIterator::chunkIterator(const dimension dims,
//							 chunkContainer* chunks,
//							 iterateMode itMode)
//	: mdItr(dims),
//	chunks_(chunks),
//	itMode_(itMode)
//{
//}

chunkIterator::chunkIterator(const self_type& mit)
	: mdItr(mit), 
	chunks_(mit.chunks_), chunkBitmap_(mit.chunkBitmap_), 
	itMode_(mit.itMode_)
{
}

chunkIterator::size_type chunkIterator::getSeqEnd()
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
}		// core
}		// msdb