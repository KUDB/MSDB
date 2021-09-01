#pragma once
#ifndef _MSDB_ARRAY_H_
#define _MSDB_ARRAY_H_

#include <pch.h>
#include <array/arrayDesc.h>
#include <array/chunkIterator.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
template <typename Dty_>
size_t calcNumItems(const Dty_* dims, const size_t dSize)
{
	size_t length = 1;
	for (size_t d = 0; d < dSize; d++)
	{
		length *= dims[d];
	}
	return length;
}

template <typename Dty_>
std::vector<Dty_> calcChunkNums(const Dty_* dims, const Dty_* chunkDims, const size_t dSize)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / chunkDims[d]);	// ceiling
	}

	return output;
}

template <typename Dty_>
std::vector<Dty_> calcChunkNums(const Dty_* dims, const Dty_* leafChunkDims, const size_t dSize, const size_t level)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / leafChunkDims[d] / pow(2, level));	// ceiling
	}

	return output;
}

template <typename Dty_>
std::vector<Dty_> calcChunkDims(const Dty_* dims, const Dty_* chunkNums, const size_t dSize)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / chunkNums[d]);	// ceiling
	}

	return output;
}

class arrayBase;
using pArray = std::shared_ptr<arrayBase>;

class arrayBase : public std::enable_shared_from_this<arrayBase>
{
public:
	using size_type = size_t;
	using chunkContainer = std::map<chunkId, pChunk>;
	using chunkPair = std::pair<chunkId, pChunk>;

public:
	arrayBase(pArrayDesc desc);
	virtual ~arrayBase();

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	arrayId getId();
	pArrayDesc getDesc();
	size_type getNumChunks();

	// Chunk
	pChunkDesc getChunkDesc(const attributeId attrId, const chunkId cId);
	pChunk getChunk(const chunkId cId);
	chunkId getChunkId(pChunkDesc cDesc);
	chunkId getChunkIdFromItemCoor(const coor& itemCoor);
	chunkId getChunkIdFromChunkCoor(const coor& chunkCoor);
	virtual coor itemCoorToChunkCoor(const coor& itemCoor);
	virtual pChunkIterator getChunkIterator(
		const iterateMode itMode = iterateMode::ALL);

	//////////////////////////////
	// Setter
	//////////////////////////////
	void setId(const arrayId id);	// only used for test
	void flush();

	virtual pChunk makeChunk(const attributeId attrId, const chunkId cId) = 0;
	// For better performance, implement the function in an inherit class.
	// The function provided by default extracts ID from ChunkDesc 
	// and re-generate chunkDesc for it.
	virtual pChunk makeChunk(const chunkDesc& desc);
	// If a chunkBit is setted, arrayBase makes a chunk accordingly.
	void makeChunks(const attributeId attrId, const bitmap& input);

	pChunk insertChunk(const attributeId attrId, pChunk inputChunk);
	template <class _Iter>
	void insertChunk(const attributeId attrId, _Iter begin, _Iter end)
	{
		for (; begin != end; ++begin)
		{
			this->chunks_.insert(chunkPair((*begin)->getId(), *begin));
			this->chunkBitmap_->setExist((*begin)->getId());
		}
	}
	virtual void freeChunk(const chunkId cId) = 0;

	cpBitmap getChunkBitmap() const;
	void copyChunkBitmap(cpBitmap chunkBitmap);
	void replaceChunkBitmap(pBitmap chunkBitmap);
	void mergeChunkBitmap(pBitmap chunkBitmap);

	void print();

protected:
	pArrayDesc desc_;
	chunkContainer chunks_;		// TODO::Seperate chunk container by attributeId
	pBitmap chunkBitmap_;		// Be initialized to false by default
};
}	// core
}	// msdb
#endif		// _MSDB_ARRAY_H_