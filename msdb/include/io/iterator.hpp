
template<typename Ty_>
conAttrItemIterator<Ty_>::conAttrItemIterator(void* data, const dimension& dims, const dimension& bSp, pBitmap itemBitmap)
	: itemIterator<Ty_>((Ty_*)data, dims), bitmapEmbeddedIterator(dims, itemBitmap), multiDimIterator(dims)
{
}
