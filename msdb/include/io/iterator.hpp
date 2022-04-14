#include <io/iterator.h>

namespace msdb
{
namespace core
{
template<typename Ty_>
iItemIterator<Ty_>::iItemIterator(const size_type dSize, iItemIterator<Ty_>::dim_const_pointer dims, pBitmap itemBitmap)
	: iItemIterator<Ty_>::base_type(dSize, dims, itemBitmap)
{
}

template<typename Ty_>
iItemIterator<Ty_>::iItemIterator(const size_type dSize, dim_const_pointer dims, dim_const_pointer startCoor, dim_const_pointer endCoor, pBitmap itemBitmap)
	: iItemIterator<Ty_>::base_type(coordinates(dSize, dims), range(coordinates(dSize, startCoor), coordinates(dSize, endCoor)), itemBitmap)
{

}

template<typename Ty_>
iItemIterator<Ty_>::iItemIterator(const coordinates& dims, pBitmap itemBitmap)
	: iItemIterator<Ty_>::base_type(dims, itemBitmap)
{
}

template<typename Ty_>
iItemIterator<Ty_>::iItemIterator(const coordinates& dims, const range& itRange, pBitmap itemBitmap)
	: iItemIterator<Ty_>::base_type(dims, itRange, itemBitmap)
{

}

template<typename Ty_>
inline iItemIterator<Ty_>::~iItemIterator()
{
}

template <typename Ty_>
void swap(iItemIterator<Ty_>& first, iItemIterator<Ty_>& second) noexcept
{
	using std::swap;

	swap(static_cast<iItemIterator<Ty_>::base_type&>(first), static_cast<iItemIterator<Ty_>::base_type&>(second));
}
}		// msdb
}		// core
