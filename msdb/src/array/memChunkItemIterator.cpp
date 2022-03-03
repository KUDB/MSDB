#include <pch.h>
#include <array/memChunkItemIterator.h>

namespace msdb
{
namespace core
{
memChunkItemIterator::memChunkItemIterator(void* data, eleType eType, const size_type dSize,
										   position_t* dims, dim_pointer csP)
	:chunkItemIterator(data, eType, dSize, dims, csP), mdItr(dSize, dims)
{
}

memChunkItemIterator::memChunkItemIterator(void* data, eleType eType, const dimension dims, const dimension csP)
	: chunkItemIterator(data, eType, dims, csP),
	mdItr(dims.size(), dims.data())
{
}

memChunkItemRangeIterator::memChunkItemRangeIterator(void* data, eleType eType, const size_type dSize,
													 dim_const_pointer sP, dim_const_pointer eP,
													 dim_const_pointer dims, dim_const_pointer csP)
	: chunkItemRangeIterator(data, eType, dSize, dims, sP, eP, csP), mdItr(dSize, dims)
{
}

memChunkItemRangeIterator::memChunkItemRangeIterator(void* data, eleType eType,
													 const dimension dims,
													 const range& range,
													 const dimension csP)
	: chunkItemRangeIterator(data, eType, dims, range, csP),
	mdItr(dims.size(), dims.data())
{
}
}		// core
}		// msdb