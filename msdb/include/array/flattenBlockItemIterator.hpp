#pragma once
#include "flattenBlockItemIterator.h"

namespace msdb
{
namespace core
{
template<typename Ty_>
flattenBufferItemIterator<Ty_>::flattenBufferItemIterator(Ty_* ptr, 
														  const flattenBufferItemIterator<Ty_>::size_type dSize, 
														  flattenBufferItemIterator<Ty_>::dim_const_pointer dims, 
														  const range& itRange,
														  pBitmap itemBitmap)
	: flattenBufferItemIterator<Ty_>::base_type(dSize, dims, itRange, itemBitmap), ptr_(ptr)
{

}

template<typename Ty_>
flattenBufferItemIterator<Ty_>::flattenBufferItemIterator(Ty_* ptr, const coordinates& dims, const range& itRange, pBitmap itemBitmap)
	: flattenBufferItemIterator<Ty_>::base_type(dims, itRange, itemBitmap), ptr_(ptr)
{

}

template<typename Ty_>
flattenBufferItemIterator<Ty_>::flattenBufferItemIterator(const flattenBufferItemIterator<Ty_>::self_type& mit)
	: flattenBufferItemIterator<Ty_>::base_type(mit), ptr_(mit.ptr_)
{

}
template<typename Ty_>
inline flattenBufferItemIterator<Ty_>::~flattenBufferItemIterator()
{
}
}		// core
}		// msdb