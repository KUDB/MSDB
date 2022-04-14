#include <pch.h>
#include <io/iterator.h>

namespace msdb
{
namespace core
{
bitmapEmbeddedIterator::bitmapEmbeddedIterator(const dimension& space, pBitmap itemBitmap)
	: base_type(space), itemBitmap_(itemBitmap)
{
	// If no bitmap were provided, the bitmap is setted into empty.
	if (itemBitmap == nullptr)
	{
		this->initBitmap(false);
	}
}

bitmapEmbeddedIterator::bitmapEmbeddedIterator(const dimension& space, const range& itRange, pBitmap itemBitmap)
	: base_type(space, itRange), itemBitmap_(itemBitmap)
{
	// If no bitmap were provided, the bitmap is setted into empty.
	if (itemBitmap == nullptr)
	{
		this->initBitmap(false);
	}
}

bitmapEmbeddedIterator::bitmapEmbeddedIterator(const dimension& space)
	: base_type(space)
{
	this->initBitmap(false);
}

bitmapEmbeddedIterator::bitmapEmbeddedIterator(const dimension& space, const range& itRange)
	: base_type(space, itRange)
{
	this->initBitmap(false);
}

bitmapEmbeddedIterator::bitmapEmbeddedIterator(const bitmapEmbeddedIterator::self_type& src)
	: base_type(src)
{
	this->itemBitmap_ = std::make_shared<bitmap>(*(src.itemBitmap_));
}

bitmapEmbeddedIterator::bitmapEmbeddedIterator(self_type&& src) noexcept
	: self_type(src.dims_, src.itemBitmap_)
{
	swap(*this, src);
}

inline bitmapEmbeddedIterator::self_type& bitmapEmbeddedIterator::operator=(const bitmapEmbeddedIterator::self_type& src)
{
	if (this == &src)
	{
		return *this;
	}

	bitmapEmbeddedIterator temp(src);
	swap(*this, temp);
	return *this;
}

inline bitmapEmbeddedIterator::self_type& bitmapEmbeddedIterator::operator=(bitmapEmbeddedIterator::self_type&& src)
{
	bitmapEmbeddedIterator temp(std::move(src));
	swap(*this, temp);
	return *this;
}

void bitmapEmbeddedIterator::initBitmap(bool flag)
{
	this->itemBitmap_ = std::make_shared<bitmap>(this->dims().area(), flag);
}
void swap(bitmapEmbeddedIterator& first, bitmapEmbeddedIterator& second) noexcept
{
	using std::swap;

	swap(static_cast<bitmapEmbeddedIterator::base_type&>(first), static_cast<bitmapEmbeddedIterator::base_type&>(second));

	swap(first.itemBitmap_, second.itemBitmap_);
}
}		// core
}		// msdb