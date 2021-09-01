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
	coorItr(dims)
	//: itemItr(data, eType, dims), 
	//blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	//coorItr(dims)
{
}

blockItemIterator::blockItemIterator(void* data,
									 const eleType eType,
									 const dimension& dims,
									 const coorRange& irange,
									 const dimension& bSp,
									 pBitmap itemBitmap)
	: blockItemRangeIterator(data, eType, dims, irange, bSp, itemBitmap),
	coorItr(dims)
	//: itemItr(data, eType, dims), 
	//blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	//coorItr(dims)
{
}

blockItemRangeIterator::blockItemRangeIterator(void* data,
											   const eleType eType,
											   const dimension& dims, 
											   const coorRange& range,
											   const dimension& bSp,
											   pBitmap itemBitmap)
	: itemRangeItr(data, eType, dims, range),
	blockItemIteratorBase(data, eType, dims, bSp, itemBitmap),
	coorItr(dims)
{
}
}		// core
}		// msdb