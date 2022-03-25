#pragma once
#ifndef _MSDB_ITERATOR_H_
#define _MSDB_ITERATOR_H_

#include <pch.h>
#include <array/dimension.h>
#include <index/bitmap.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class bitmapEmbeddedIterator : public multiDimIterator
{
public:
	using base_type = multiDimIterator;

	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	// nullptr bitmap is initialized into fully empty bitmap.
	bitmapEmbeddedIterator(const dimension& space, pBitmap itemBitmap);
	bitmapEmbeddedIterator(const dimension& space, const range& itRange, pBitmap itemBitmap);
	bitmapEmbeddedIterator(const dimension& space);
	bitmapEmbeddedIterator(const dimension& space, const range& itRange);

public:
	_NODISCARD inline bool isExist() const
	{
		return this->itemBitmap_->isExist(this->seqPos_);
	}
	_NODISCARD inline bool isExist(const size_t seqPos) const
	{
		return this->itemBitmap_->isExist(seqPos);
	}
	inline void setExist()
	{
		this->itemBitmap_->setExist(this->seqPos_);
	}
	inline void setExist(const size_t seqPos)
	{
		this->itemBitmap_->setExist(seqPos);
	}
	inline void setNull(const size_t seqPos)
	{
		this->itemBitmap_->setNull(seqPos);
	}
	inline void setNull()
	{
		this->itemBitmap_->setNull(this->seqPos_);
	}

protected:
	void initBitmap(bool flag);

protected:
	pBitmap itemBitmap_;
};

template <typename Ty_>
class iItemIterator : public bitmapEmbeddedIterator
{
public:
	using self_type = iItemIterator<Ty_>;
	using base_type = bitmapEmbeddedIterator;
	using data_type = Ty_;

	using size_type = size_t;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	iItemIterator(const size_type dSize, dim_const_pointer dims, pBitmap itemBitmap);
	iItemIterator(const size_type dSize, dim_const_pointer dims, dim_const_pointer startCoor, dim_const_pointer endCoor, pBitmap itemBitmap);
	iItemIterator(const coordinates& dims, pBitmap itemBitmap);
	iItemIterator(const coordinates& dims, const range& itRange, pBitmap itemBitmap);
	iItemIterator(const self_type& mit);

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	virtual data_type& getAtSeqPos(const position_t& seqPos) = 0;
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// Pointer
	virtual data_type& operator*() = 0;
	virtual data_type* operator->() = 0;
	//////////////////////////////
};

//////////////////////////////
// Iterator with null bitmap
// 
// virtual inherit 'multiDimIterator'

using vpItemIterator = std::variant<std::shared_ptr<iItemIterator<int8_t>>, std::shared_ptr<iItemIterator<int16_t>>,
	std::shared_ptr<iItemIterator<int32_t>>, std::shared_ptr<iItemIterator<int64_t>>,
	std::shared_ptr<iItemIterator<uint8_t>>, std::shared_ptr<iItemIterator<uint16_t>>,
	std::shared_ptr<iItemIterator<uint32_t>>, std::shared_ptr<iItemIterator<uint64_t>>,
	std::shared_ptr<iItemIterator<float>>, std::shared_ptr<iItemIterator<double>>,
	std::shared_ptr<iItemIterator<bool>>>;
}
}

#include "iterator.hpp"

#endif	// _MSDB_ITERATOR_H_