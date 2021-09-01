#pragma once
#ifndef _MSDB_BITMAP_H_
#define _MSDB_BITMAP_H_

#include <pch.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class bitmap;
using pBitmap = std::shared_ptr<bitmap>;
using cpBitmap = std::shared_ptr<const bitmap>;

class bitmapTree;
using pBitmapTree = std::shared_ptr<bitmapTree>;
using cpBitmapTree = std::shared_ptr<const bitmapTree>;

class bitmap
{
public:
	bitmap(const size_t capacity, const bool value = false);
	//bitmap(const coor& space, const bool value = false);
	bitmap(const bitmap& mit);

public:
	inline bool isExist(const size_t seqPos) const
	{
		assert(seqPos < this->data_.size());
		return this->data_[seqPos];
	}

	inline void setExist(const size_t seqPos)
	{
		assert(seqPos < this->data_.size());
		this->data_[seqPos] = true;
	}

	inline void setNull(const size_t seqPos)
	{
		assert(seqPos < this->data_.size());
		this->data_[seqPos] = false;
	}

	inline size_t getCapacity() const
	{
		return this->data_.size();
	}

	virtual bool isTree() const;
	
	inline bool isEmpty()
	{
		for(int i = 0 ; i < this->data_.size(); ++i)
		{
			if(this->data_[i])
			{
				return false;
			}
		}

		return true;
	}

	void andMerge(bitmap& mit);
	void orMerge(bitmap& mit);

	void print() const;
public:
	std::vector<bool>::reference operator[](size_t seqPos);
	const bool& operator[](size_t seqPos) const;

protected:
	std::vector<bool> data_;
};

class bitmapTree : public bitmap
{
public:
	bitmapTree(const size_t capacity, const bool value = false);
	bitmapTree(const bitmapTree& mit);

public:
	pBitmap makeChild(const size_t seqPos, const size_t capacity, const bool value = false);
	pBitmap getChild(const size_t seqPos);
	cpBitmap getChild(const size_t seqPos) const;
	bool hasChild(const size_t seqPos) const;

	virtual bool isTree();

protected:
	std::vector<pBitmap> childs_;
};
}		// core
}		// msdb
#endif	// _MSDB_BITMAP_H_