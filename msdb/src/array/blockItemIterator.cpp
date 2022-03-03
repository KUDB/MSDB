#include <pch.h>
#include <array/blockItemIterator.h>

namespace msdb
{
namespace core
{
blockItemIterator::blockItemIterator(void* data, 
									 const eleType eType,
									 const dimension& dims,
									 const dimension& bSp,
									 pBitmap itemBitmap)
	: blockItemRangeIterator(data, eType, dims, dims, bSp, itemBitmap),
	mdItr(dims)
	//: itemItr(data, eType, dims), 
	//blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	//mdItr(dims)
{
}

blockItemIterator::blockItemIterator(void* data,
									 const eleType eType,
									 const dimension& dims,
									 const range& irange,
									 const dimension& bSp,
									 pBitmap itemBitmap)
	: blockItemRangeIterator(data, eType, dims, irange, bSp, itemBitmap),
	mdItr(dims)
	//: itemItr(data, eType, dims), 
	//blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	//mdItr(dims)
{
}

blockItemRangeIterator::blockItemRangeIterator(void* data,
											   const eleType eType,
											   const dimension& dims, 
											   const range& range,
											   const dimension& bSp,
											   pBitmap itemBitmap)
	: itemRangeItr(data, eType, dims, range),
	blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	mdItr(dims)
{
}
}		// core
}		// msdb