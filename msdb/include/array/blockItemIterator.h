#pragma once
#ifndef _MSDB_BLOCKITEM_ITERATOR_H_
#define _MSDB_BLOCKITEM_ITERATOR_H_

#include <pch.h>
#include <array/dimension.h>
#include <index/bitmap.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class blockItemIterator;
class blockItemRangeIterator;
using pBlockItemIterator = std::shared_ptr<blockItemIterator>;
using pBlockItemRangeIterator = std::shared_ptr<blockItemRangeIterator>;

template <typename Dty_>
class blockItemIteratorBase : virtual public coordinateIterator<Dty_>
{
public:
	using base_type = coordinateIterator<Dty_>;
	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	blockItemIteratorBase(void* data, 
						  const eleType eType, 
						  const dimension& dims,
						  const dimension& bSp,
						  pBitmap itemBitmap)
		: base_type(dims), bSp_(bSp), itemBitmap_(itemBitmap)
	{
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
	coordinate_type bSp_;
	pBitmap itemBitmap_;
};

using blockItemItrBase = blockItemIteratorBase<position_t>;

class blockItemRangeIterator : public itemRangeItr, public blockItemItrBase
{
public:
	using base_type = itemRangeItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;


public:
	blockItemRangeIterator(void* data, const eleType eType,
						   const dimension& dims,
						   const coorRange& range,
						   const dimension& bSp,
						   pBitmap itemBitmap);
};

class blockItemIterator : public blockItemRangeIterator
{
public:
	using self_type = blockItemRangeIterator;
	using base_type = itemItr;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	blockItemIterator(void* data, const eleType eType,
					  const dimension& dims, const dimension& bSp,
					  pBitmap itemBitmap);

	blockItemIterator(void* data, const eleType eType,
					  const dimension& dims, const coorRange& irange,
					  const dimension& bSp,
					  pBitmap itemBitmap);
};
}		// core
}		// msdb
#endif	// _MSDB_BLOCKITEM_ITERATOR_H_
