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
flattenBufferItemIterator<Ty_>::flattenBufferItemIterator(const flattenBufferItemIterator<Ty_>& src)
	: flattenBufferItemIterator<Ty_>::base_type(src), ptr_(src.ptr_)
{

}
template<typename Ty_>
inline flattenBufferItemIterator<Ty_>::flattenBufferItemIterator(flattenBufferItemIterator<Ty_>&& src) noexcept
{
	swap(*this, src);
}
template<typename Ty_>
inline flattenBufferItemIterator<Ty_>::~flattenBufferItemIterator()
{
}
template<typename Ty_>
inline flattenBufferItemIterator<Ty_>& flattenBufferItemIterator<Ty_>::operator=(const flattenBufferItemIterator<Ty_>& src)
{
	if (this == &src)
	{
		return *this;
	}

	flattenBufferItemIterator<Ty_> temp(src);
	swap(*this, temp);
	return *this;
}
template<typename Ty_>
inline flattenBufferItemIterator<Ty_>& flattenBufferItemIterator<Ty_>::operator=(flattenBufferItemIterator<Ty_>&& src)
{
	flattenBufferItemIterator<Ty_> temp(std::move(src));
	swap(*this, temp);
	return *this;
}
template<typename Ty_>
void swap(flattenBufferItemIterator<Ty_>& first, flattenBufferItemIterator<Ty_>& second) noexcept
{
	using std::swap;

	swap(static_cast<flattenBufferItemIterator<Ty_>::base_type&>(first), static_cast<flattenBufferItemIterator<Ty_>::base_type&>(second));

	swap(first.ptr_, second.ptr_);
}
}		// core
}		// msdb