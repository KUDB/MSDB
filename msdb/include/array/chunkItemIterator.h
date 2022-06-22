#pragma once
#ifndef _MSDB_CHUNKITEMITERATOR_H_
#define _MSDB_CHUNKITEMITERATOR_H_

#include <pch.h>
#include <util/coordinate.h>
#include <array/dimension.h>

namespace msdb
{
namespace core
{
class chunkItemIterator;
class chunkItemRangeIterator;
using pChunkItemIterator = std::shared_ptr<chunkItemIterator>;
using pChunkItemRangeIterator = std::shared_ptr<chunkItemRangeIterator>;

class chunkItemIteratorBase : virtual public multiDimIterator
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
	chunkItemIteratorBase(void* data, eleType eType, const size_type dSize,
						  dim_const_pointer dims, dim_const_pointer csP)
		: base_type(dSize, dims)
	{
		this->csP_ = new dim_type[dSize];
		this->memcpyDim(this->csP_, csP);
	}

public:
	coordinates coorOut2In(coordinates& out)
	{
		coordinates in(this->dSize());
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			in[d] = out[d] - this->csP_[d];
		}

		return in;
	}

	coordinates coorIn2Out(coordinates& in)
	{
		coordinates out(this->dSize());
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			out[d] = in[d] + this->csP_[d];
		}
		return out;
	}

	coordinates coorIn2Out()
	{
		return this->coorIn2Out(this->coor_);
	}

	coordinates ceP()
	{
		coordinates ceP(this->dSize());
		for (dimensionId d = 0; d < this->dSize(); d++)
		{
			ceP[d] = this->csP_[d] + this->dims_[d];
		}

		return ceP;
	}

	coordinates outCoor()
	{
		return this->coorIn2Out();
	}

	coordinates inCoor()
	{
		return this->coor_;
	}

protected:
	dim_pointer csP_;			// Chunk start point
};

class chunkItemIterator : public itemItr, public chunkItemIteratorBase
{
public:
	using self_type = chunkItemIterator;
	using base_type = itemItr;

	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;

public:
	//chunkItemIterator();

	chunkItemIterator(void* data, const eleType eType,
					  const size_type dSize,
					  dim_const_pointer dims, dim_const_pointer csP);

	chunkItemIterator(void* data, const eleType eType, const dimension dims, const dimension csP);
};

class chunkItemRangeIterator : public itemRangeItr, public chunkItemIteratorBase
{
public:
	using base_type = itemRangeItr;

	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;


public:
	chunkItemRangeIterator(void* data, const eleType eType, 
						   const size_type dSize, dim_const_pointer dims,
						   dim_const_pointer sP, dim_const_pointer eP,
						   dim_const_pointer csP);

	chunkItemRangeIterator(void* data, const eleType eType, 
						   const dimension dims, 
						   const range& range,
						   const dimension csP);
};
}		// core
}		// msdb
#endif	// _MSDB_CHUNKITEMITERATOR_H_