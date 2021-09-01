#include <pch.h>
#include <index/bitmap.h>

namespace msdb
{
namespace core
{
bitmap::bitmap(const size_t capacity, const bool value)
	: data_(capacity, value)
{
}
bitmap::bitmap(const bitmap& mit)
	: data_(mit.data_)
{
}

bool bitmap::isTree() const
{
	return false;
}

void bitmap::andMerge(bitmap& mit)
{
	assert(mit.getCapacity() >= this->getCapacity());
	size_t capacity = this->getCapacity();
	for(size_t i = 0; i < capacity; ++i)
	{
		this->data_[i] = (bool)this->data_[i] & (bool)mit[i];
	}
}

void bitmap::orMerge(bitmap& mit)
{
	assert(mit.getCapacity() >= this->getCapacity());
	size_t capacity = this->getCapacity();
	for (size_t i = 0; i < capacity; ++i)
	{
		this->data_[i] = (bool)this->data_[i] | (bool)mit[i];
	}
}

void bitmap::print() const
{
	size_t capacity = this->getCapacity();
	std::stringstream ss;
	for (size_t i = 0; i < capacity; ++i)
	{
		ss << "[" << i << "]: ";
		if(this->data_[i])
		{
			ss << "T / ";
		}else
		{
			ss << "F / ";
		}
	}

	BOOST_LOG_TRIVIAL(debug) << ss.str();
}

std::vector<bool>::reference bitmap::operator[](size_t seqPos)
{
	assert(seqPos < this->data_.size());
	return this->data_[seqPos];
}

const bool& bitmap::operator[](size_t seqPos) const
{
	assert(seqPos < this->data_.size());
	return this->data_[seqPos];
}

bitmapTree::bitmapTree(const size_t capacity, const bool value)
	: bitmap(capacity, value), childs_(capacity, nullptr)
{

}

bitmapTree::bitmapTree(const bitmapTree& mit)
	: bitmap(mit), childs_(mit.childs_)
{
	for(auto node : mit.childs_)
	{
		if(node)
		{
			node = std::make_shared<bitmap>(*node);
		}
	}
}

pBitmap bitmapTree::makeChild(const size_t seqPos, const size_t capacity, const bool value)
{
	pBitmap child = std::make_shared<bitmap>(capacity, value);
	this->childs_[seqPos] = child;
	return child;
}

pBitmap bitmapTree::getChild(const size_t seqPos)
{
	assert(seqPos < this->childs_.size());
	return this->childs_[seqPos];
}

bool bitmapTree::hasChild(const size_t seqPos) const
{
	if(seqPos >= this->childs_.size() || this->childs_[seqPos] == nullptr)
	{
		return false;
	}

	return true;
}

cpBitmap bitmapTree::getChild(const size_t seqPos) const
{
	assert(seqPos < this->childs_.size());
	return this->childs_[seqPos];
}

bool bitmapTree::isTree()
{
	return true;
}
}		// core
}		// msdb