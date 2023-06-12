#pragma once
#ifndef _MSDB_BLOCK_H_
#define _MSDB_BLOCK_H_

#include <pch.h>
#include <array/blockItemIterator.h>
#include <array/blockBuffer.h>
#include <array/blockDesc.h>
#include <io/bitstream.h>
#include <util/logger.h>
#include <io/iterator.h>

namespace msdb
{
namespace core
{
class chunk;
class block;
using pBlock = std::shared_ptr<block>;

class block : public std::enable_shared_from_this<block>
{
public:
	block(pBlockDesc desc);
	virtual ~block();

//////////////////////////////
// Desc
//////////////////////////////
public:
	blockId getId() const;
	pBlockDesc getDesc();
	const pBlockDesc getDesc() const;
	dimensionId getDSize() const;
	//inline void setIsp(coor isp);
	//inline void setIep(coor iep);
	range getBlockRange() const;
	range getBlockItemRange() const;
	void setBlockDesc(const pBlockDesc inDesc);

	virtual void serialize(bstream& os) = 0;
	virtual void deserialize(bstream& is) = 0;

protected:
	pBlockDesc desc_;

//////////////////////////////
// Buffer
//////////////////////////////
public:
	virtual void copy(pBlock sBlock);
	bool isMaterialized() const;

	// Shallow copy
	// A block buffer only references a part of a chunk buffer memory.
	virtual void unlinkFromChunkBuffer();
	virtual void refChunkBufferWithoutOwnership(void* data, const bufferSize size) = 0;	// used in chunk
public:
	pBlockBuffer getBuffer();
	virtual bool operator== (const block& rhs) const;

	friend class chunk;

protected:
	pBlockBuffer cached_;	// hold materialized block
	
//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	// TODO
	//[[deprecated("")]] 
	virtual pBlockItemIterator getItemIterator() = 0;
	virtual const pBlockItemIterator getItemIterator() const = 0;
	// TODO
	//[[deprecated("")]] 
	virtual pBlockItemRangeIterator getItemRangeIterator(const range& range) = 0;
	virtual const pBlockItemRangeIterator getItemRangeIterator(const range& range) const = 0;

	virtual vpItemIterator getValueIterator() = 0;
	virtual vpItemIterator getValueRangeIterator(const range& range) = 0;

	void copyBitmap(cpBitmap itemBitmap);
	void replaceBitmap(pBitmap itemBitmap);
	void mergeBitmap(pBitmap itemBitmap);
	void initEmptyBitmap();
	pBitmap getBitmap();

protected:
	pBitmap itemBitmap_;

//////////////////////////////
// Print
//////////////////////////////
public:
	void print();

protected:
	template <class Ty_>
	void printImp()
	{
		auto iit = this->getItemIterator();
		std::stringstream ss;
		ss << "Block [" << this->desc_->blockCoor_.toString() << " (" << static_cast<int64_t>(this->getId()) << ")]";
		int64_t row = -1;
		while(!iit->isEnd())
		{
			int64_t curRow = iit->coor()[0];
			if (row != curRow)
			{
				ss << "\n";
				ss << curRow << ": ";
				row = curRow;
			}
			if(iit->isExist())
			{
				ss << (**iit).get<Ty_>() << ", ";
			}else
			{
				ss << "*, ";
			}
			++(*iit);
		}
		BOOST_LOG_TRIVIAL(debug) << ss.str() << "";
	}
	template<>
	void printImp<char>()
	{
		auto iit = this->getItemIterator();
		std::stringstream ss;
		ss << "Block [" << this->desc_->blockCoor_.toString() << " (" << static_cast<int64_t>(this->getId()) << ")]";
		int64_t row = -1;
		while (!iit->isEnd())
		{
			int64_t curRow = iit->coor()[0];
			if(row != curRow)
			{
				ss << "\n";
				ss << curRow << " : ";
				row = curRow;
			}
			if (iit->isExist())
			{
				ss << static_cast<int>((**iit).get<char>()) << ", ";
			} else
			{
				ss << "*, ";
			}
			++(*iit);
		}
		BOOST_LOG_TRIVIAL(debug) << ss.str() << "";
	}

	template<>
	void printImp<unsigned char>()
	{
		auto iit = this->getItemIterator();
		std::stringstream ss;
		ss << "Block [" << this->desc_->blockCoor_.toString() << " (" << static_cast<int64_t>(this->getId()) << ")]";
		int64_t row = -1;
		while (!iit->isEnd())
		{
			int64_t curRow = iit->coor()[0];
			if (row != curRow)
			{
				ss << "\n";
				ss << curRow << " : ";
				row = curRow;
			}
			if (iit->isExist())
			{
				ss << static_cast<unsigned int>((**iit).get<unsigned char>()) << ", ";
			} else
			{
				ss << "*, ";
			}
			++(*iit);
		}
		BOOST_LOG_TRIVIAL(debug) << ss.str() << "";
	}
};
}		// core
}		// msdb
#endif