#pragma once
#ifndef _MSDB_MEMBLOCKITEMITERATOR_H_
#define _MSDB_MEMBLOCKITEMITERATOR_H_

#include <pch.h>
#include <array/blockItemIterator.h>
#include <index/bitmap.h>

namespace msdb
{
namespace core
{
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
}		// core
}		// msdb
#endif	// _MSDB_MEMBLOCKITEMITERATOR_H_

