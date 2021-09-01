#pragma once
#ifndef _MSDB_MEMCHUNKITEMITERATOR_H_
#define _MSDB_MEMCHUNKITEMITERATOR_H_

#include <pch.h>
#include <array/chunkItemIterator.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class memChunkItemIterator : public chunkItemIterator
{
public:
	using self_type = memChunkItemIterator;
	using base_type = chunkItemIterator;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;
public:
	// csP : chunk start point
	memChunkItemIterator(void* data, eleType eType, const size_type dSize, position_t* dims,
						 dim_pointer csP);

	memChunkItemIterator(void* data, eleType eType,
						 const dimension dims,
						 const dimension csP);
};

class memChunkItemRangeIterator : public chunkItemRangeIterator
{
public:

	memChunkItemRangeIterator(void* data, eleType eType, const size_type dSize,
							  dim_const_pointer sP, dim_const_pointer eP,
							  dim_const_pointer dims, dim_const_pointer csP);

	memChunkItemRangeIterator(void* data, eleType eType,
							  const dimension dims,
							  const coorRange& range,
							  const dimension csP);
};
}		// core
}		// msdb
#endif	// _MSDB_MEMCHUNKITEMITERATOR_H_
