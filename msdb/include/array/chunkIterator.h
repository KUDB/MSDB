#pragma once
#ifndef _MSDB_CHUNKITERATOR_H_
#define _MSDB_CHUNKITERATOR_H_

#include <pch.h>
#include <array/chunkContainer.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class chunkIterator;
using pChunkIterator = std::shared_ptr<chunkIterator>;

class chunkIterator : public coorItr
{
public:
	using self_type = chunkIterator;
	using base_type = coorItr;

	using size_type = coorItr::size_type;

public:
	chunkIterator(const dimension dims,
				  chunkContainer* chunks, pBitmap chunkBitmap, 
				  iterateMode itMode);

	//chunkIterator(const dimension dims,
	//			  chunkContainer* chunks,
	//			  iterateMode itMode);

	chunkIterator(const self_type& mit);

public:
	size_type getSeqEnd();
	bool isExist();
	bool isExist(chunkId cid);
	iterateMode getIterateMode();

	//////////////////////////////
	// Iterating
	//////////////////////////////
	virtual void next()
	{
		base_type::next();

		if (this->itMode_ == iterateMode::EXIST)
		{
			while (!this->isExist() && !this->isEnd())
			{
				base_type::next();
			}
		}
	}
	virtual void prev()
	{
		base_type::prev();

		if (this->itMode_ == iterateMode::EXIST)
		{
			while (!this->isExist() && !this->isFront())
			{
				base_type::prev();
			}
		}
	}

	//////////////////////////////
	// Operators
	//////////////////////////////
	pChunk operator*() { return this->chunks_->at((chunkId)this->seqPos_); }
	pChunk operator->() { return this->chunks_->at((chunkId)this->seqPos_); }

protected:
	chunkContainer* chunks_;
	pBitmap chunkBitmap_;
	iterateMode itMode_;
};
}		// core
}		// msdb
#endif	// _MSDB_CHUNKITERATOR_H_