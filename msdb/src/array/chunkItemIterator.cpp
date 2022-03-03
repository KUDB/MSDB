#include <pch.h>
#include <array/chunkItemIterator.h>

namespace msdb
{
namespace core
{
chunkItemIterator::chunkItemIterator(void* data, const eleType eType,
									 const size_type dSize,
									 dim_const_pointer dims, dim_const_pointer csP)
	: itemItr(data, eType, dSize, dims),
	chunkItemIteratorBase(data, eType, dSize, dims, csP),
	mdItr(dSize, dims)
{
}

chunkItemIterator::chunkItemIterator(void* data, const eleType eType,
									 const dimension dims, const dimension csP)
	: itemItr(data, eType, dims),
	chunkItemIteratorBase(data, eType, dims.size(), dims.data(), csP.data()),
	mdItr(dims.size(), dims.data())
{
	assert(dims.size() == csP.size());
}

chunkItemRangeIterator::chunkItemRangeIterator(void* data, const eleType eType, const size_type dSize,
											   const dim_const_pointer dims, 
											   const dim_const_pointer sP, const dim_const_pointer eP,
											   const dim_const_pointer csP)
	: itemRangeItr(data, eType, dSize, dims, sP, eP),
	chunkItemIteratorBase(data, eType, dSize, dims, csP),
	mdItr(dSize, dims)
{
}

chunkItemRangeIterator::chunkItemRangeIterator(void* data, const eleType eType, 
											   const dimension dims,
											   const range& range,
											   const dimension csP)
	: itemRangeItr(data, eType, dims, range),
	chunkItemIteratorBase(data, eType, dims.size(), dims.data(), csP.data()),
	mdItr(dims.size(), dims.data())
{
	assert(dims.size() == csP.size());
}
}		// core
}		// msdb