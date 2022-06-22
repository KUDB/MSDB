#pragma once
#ifndef _MSDB_MEMBLOCKITEMITERATOR_H_
#define _MSDB_MEMBLOCKITEMITERATOR_H_

#include <pch.h>
#include <array/blockItemIterator.h>
#include <index/bitmap.h>
#include <io/iterator.h>

namespace msdb
{
namespace core
{
// Deprecated
class flattenBlockItemIterator : public blockItemIterator
{
public:
	using self_type = flattenBlockItemIterator;
	using base_type = blockItemIterator;

	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;
public:
	// bSp : block start point
	flattenBlockItemIterator(void* data, 
						 const eleType eType, 
						 const dimension& dims,
						 const dimension& bSp,
						 pBitmap itemBitmap);

	flattenBlockItemIterator(void* data,
						 const eleType eType,
						 const dimension& dims,
						 const range& irange,
						 const dimension& bSp,
						 pBitmap itemBitmap);
};

class flattenBlockItemRangeIterator : public blockItemRangeIterator
{
public:
	flattenBlockItemRangeIterator(void* data,
							  const eleType eType,
							  const dimension& dims,
							  const range& range,
							  const dimension& bSp,
							  pBitmap itemBitmap);
};

template <typename Ty_>
class flattenBufferItemIterator : public iItemIterator<Ty_>
{
public:
	using self_type = flattenBufferItemIterator<Ty_>;
	using base_type = iItemIterator<Ty_>;
	using data_type = Ty_;

	using size_type = size_t;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	flattenBufferItemIterator(Ty_* ptr, const size_type dSize, dim_const_pointer dims, const range& itRange, pBitmap itemBitmap);
	flattenBufferItemIterator(Ty_* ptr, const coordinates& dims, const range& itRange, pBitmap itemBitmap);
	flattenBufferItemIterator(const flattenBufferItemIterator<Ty_>& src);
	flattenBufferItemIterator(flattenBufferItemIterator<Ty_>&& src) noexcept;

	virtual ~flattenBufferItemIterator();

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	virtual data_type& getAtSeqPos(const position_t& seqPos)
	{
		return *(ptr_ + seqPos);
	}
	//////////////////////////////

	//////////////////////////////
	// Operators
	//////////////////////////////
	// Assign
	friend void swap(flattenBufferItemIterator<Ty_>& first, flattenBufferItemIterator<Ty_>& second) noexcept;
	flattenBufferItemIterator<Ty_>& operator=(const flattenBufferItemIterator<Ty_>& src);
	flattenBufferItemIterator<Ty_>& operator=(flattenBufferItemIterator<Ty_>&& src);
	// Comparison
	virtual bool operator==(const self_type& rhs) const { return ptr_ == rhs.ptr_ && this->seqPos_ == rhs.seqPos_; }
	virtual bool operator!=(const self_type& rhs) const { return ptr_ != rhs.ptr_ || this->seqPos_ != rhs.seqPos_; }
	// Pointer
	virtual data_type& operator*() { return *(ptr_ + this->seqPos_); }
	virtual data_type* operator->() { return (data_type*)(ptr_ + this->seqPos_); }
	//////////////////////////////

protected:
	Ty_* ptr_;			// pointer to element
						// set pointer type as char, to move one byte for each seqPos_
};

template <typename Ty_>
void swap(flattenBufferItemIterator<Ty_>& first, flattenBufferItemIterator<Ty_>& second) noexcept;
}		// core
}		// msdb

#include "flattenBlockItemIterator.hpp"

#endif	// _MSDB_MEMBLOCKITEMITERATOR_H_

