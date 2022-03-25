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

void bitmapEmbeddedIterator::initBitmap(bool flag)
{
	this->itemBitmap_ = std::make_shared<bitmap>(this->dims().area(), flag);
}
}		// core
}		// msdb