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
class constChunkIterator;
using pChunkIterator = std::shared_ptr<chunkIterator>;
using pConstChunkIterator = std::shared_ptr<constChunkIterator>;

class chunkIterator : public mdItr
{
public:
	using self_type = chunkIterator;
	using base_type = mdItr;

	using size_type = mdItr::size_type;

public:
	chunkIterator(const dimension dims,
				  chunkContainer* chunks, pBitmap chunkBitmap, 
				  iterateMode itMode);

	chunkIterator(const self_type& mit);

public:
	size_type getSeqEnd() const;
	bool isExist() const;
	bool isExist();
	bool isExist(const chunkId cid) const;
	// Bitmap is set, but chunk is not materialized
	bool needToMake() const;
	iterateMode getIterateMode() const;
	const pBitmap getChunkBitmap() const;

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

class constChunkIterator : public mdItr
{
public:
	using self_type = constChunkIterator;
	using base_type = mdItr;

	using size_type = mdItr::size_type;

public:
	constChunkIterator(const dimension dims,
		const chunkContainer* chunks, const pBitmap chunkBitmap,
		iterateMode itMode);

	constChunkIterator(const self_type& mit);

public:
	size_type getSeqEnd() const;
	bool isExist() const;
	bool isExist(const chunkId cid) const;
	// Bitmap is set, but chunk is not materialized
	bool needToMake() const;
	iterateMode getIterateMode() const;
	const pBitmap getChunkBitmap() const;

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
	const pChunk operator*() { return this->chunks_->at((chunkId)this->seqPos_); }
	const pChunk operator->() { return this->chunks_->at((chunkId)this->seqPos_); }

protected:
	const chunkContainer* chunks_;
	const pBitmap chunkBitmap_;
	iterateMode itMode_;
};
}		// core
}		// msdb
#endif	// _MSDB_CHUNKITERATOR_H_