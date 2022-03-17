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
//////////////////////////////
// Iterator with null bitmap
// 
// virtual inherit 'multiDimIterator'

class bitmapEmbeddedIterator : virtual public multiDimIterator
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
	bitmapEmbeddedIterator(const dimension& dims,
						  pBitmap itemBitmap)
		: base_type(dims), itemBitmap_(itemBitmap)
	{
	}

	// If no bitmap were provided, the bitmap is setted into empty.
	bitmapEmbeddedIterator(void* data,
						   const eleType eType,
						   const dimension& dims)
		: base_type(dims)
	{
		this->itemBitmap_ = std::make_shared<bitmap>(dims.area(), false);
	}

public:
	bool isExist() const
	{
		return this->itemBitmap_->isExist(this->seqPos_);
	}
	bool isExist(const size_t seqPos) const
	{
		return this->itemBitmap_->isExist(seqPos);
	}
	void setExist()
	{
		this->itemBitmap_->setExist(this->seqPos_);
	}
	void setExist(const size_t seqPos)
	{
		this->itemBitmap_->setExist(seqPos);
	}
	void setNull(const size_t seqPos)
	{
		this->itemBitmap_->setNull(seqPos);
	}
	void setNull()
	{
		this->itemBitmap_->setNull(this->seqPos_);
	}

protected:
	pBitmap itemBitmap_;
};

template <typename Ty_>
class conAttrItemIterator : public itemIterator<Ty_>, public bitmapEmbeddedIterator
{
public:
	using base_type = itemIterator<Ty_>;

public:
	conAttrItemIterator(void* data, const dimension& dims, 
						const dimension& bSp, pBitmap itemBitmap);
};

#include "iterator.hpp"

using abAttrItemIterator = std::variant<conAttrItemIterator<int8_t>, conAttrItemIterator<int16_t>, conAttrItemIterator<int32_t>, conAttrItemIterator<int64_t>,
	conAttrItemIterator<uint8_t>, conAttrItemIterator<uint16_t>, conAttrItemIterator<uint32_t>, conAttrItemIterator<uint64_t>,
	conAttrItemIterator<float>, conAttrItemIterator<double>, conAttrItemIterator<bool>>;
}
}

#endif	// _MSDB_ITERATOR_H_