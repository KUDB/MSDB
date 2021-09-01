#include <pch.h>
#include <array/chunkIterator.h>

namespace msdb
{
namespace core
{
chunkIterator::chunkIterator(const dimension dims,
							 chunkContainer* chunks, pBitmap chunkBitmap, 
							 iterateMode itMode)
	: coorItr(dims), 
	chunks_(chunks), chunkBitmap_(chunkBitmap),
	itMode_(itMode)
{
}

//chunkIterator::chunkIterator(const dimension dims,
//							 chunkContainer* chunks,
//							 iterateMode itMode)
//	: coorItr(dims),
//	chunks_(chunks),
//	itMode_(itMode)
//{
//}

chunkIterator::chunkIterator(const self_type& mit)
	: coorItr(mit), 
	chunks_(mit.chunks_), chunkBitmap_(mit.chunkBitmap_), 
	itMode_(mit.itMode_)
{
}

chunkIterator::size_type chunkIterator::getSeqEnd()
{
	return this->chunks_->size();
}

bool chunkIterator::isExist()
{
	return this->isExist(this->seqPos_);
}

bool chunkIterator::isExist(chunkId cid)
{
	return (*this->chunkBitmap_)[cid];
	//&& this->chunks_->find(cid) != this->chunks_->end();
}

iterateMode chunkIterator::getIterateMode()
{
	return this->itMode_;
}
}		// core
}		// msdb