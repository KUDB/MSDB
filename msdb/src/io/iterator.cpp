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

bitmapEmbeddedIterator::bitmapEmbeddedIterator(const bitmapEmbeddedIterator& mit)
	: base_type(mit)
{
	// TODO::bitmapEmbeddedIterator(const self_type& mit)
	//this->itemBitmap_ = std::make_shared<bitmap>(*(mit.itemBitmap_));
}

inline bitmapEmbeddedIterator::self_type& bitmapEmbeddedIterator::operator=(const bitmapEmbeddedIterator::self_type& rhs)
{
	// TODO::bitmapEmbeddedIterator::Perform assign operator for super class
	//this->itemBitmap_ = std::make_shared<bitmap>(*(rhs.itemBitmap_);

	return *this;
}

void bitmapEmbeddedIterator::initBitmap(bool flag)
{
	this->itemBitmap_ = std::make_shared<bitmap>(this->dims().area(), flag);
}
}		// core
}		// msdb