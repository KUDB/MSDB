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
iItemIterator<Ty_>::iItemIterator(const iItemIterator<Ty_>::self_type& mit)
	: iItemIterator<Ty_>::base_type(mit)
{
}

template<typename Ty_>
inline iItemIterator<Ty_>::~iItemIterator()
{
}
template<typename Ty_>
inline iItemIterator<Ty_>& iItemIterator<Ty_>::operator=(const iItemIterator<Ty_>& rhs)
{
	// TODO: 여기에 return 문을 삽입합니다.
}
}		// msdb
}		// core
