#pragma once
#ifndef _MSDB_ARRAY_H_
#define _MSDB_ARRAY_H_

#include <pch.h>
#include <array/arrayDesc.h>
#include <array/chunkIterator.h>
#include <util/coordinate.h>
#include <array/arrayUtil.h>
#include <array/chunkContainer.h>
#include <array/chunkFactory.h>

namespace msdb
{
namespace core
{
class array;
using pArray = std::shared_ptr<array>;

class array : public std::enable_shared_from_this<array>
{
public:
	using size_type = size_t;
	//using chunkContainer = std::map<attributeId, std::map<chunkId, pChunk>>;
	using bitmapContainer = std::map<attributeId, pBitmap>;
	using chunkPair = std::pair<chunkId, pChunk>;

public:
	array(pArrayDesc desc);
	virtual ~array();

public:
	// Deep clone for desc objects,
	// Shallow clone for chunks and cached memories
	virtual pArray shallowClone(const bool takeOwnership = false) = 0;

public:
	//////////////////////////////
	// Getter
	//////////////////////////////
	arrayId getId();
	pArrayDesc getDesc();
	//size_type getNumChunks();

	// Chunk
	pChunkDesc getChunkDesc(const attributeId attrId, const chunkId cId);
	pChunk getChunk(const attributeId attrId, const chunkId cId);
	chunkId getChunkId(pChunkDesc cDesc);
	virtual chunkId itemCoorToChunkId(const coor& itemCoor);
	virtual chunkId chunkCoorToChunkId(const coor& chunkCoor);
	virtual coor itemCoorToChunkCoor(const coor& itemCoor);
	virtual pChunkIterator getChunkIterator(const attributeId attrId, 
		const iterateMode itMode = iterateMode::ALL);

protected:
	pChunkFactory getChunkFactory(const attributeId& attrId);

public:
	//////////////////////////////
	// Setter
	//////////////////////////////
	void setId(const arrayId id);	// only used for test
	void flush();

	// Initialize and assign ChunkFactory in this->chunkFactories_ according to data type of each attribute
	virtual void initChunkFactories() = 0;
	inline pChunk makeChunk(const attributeId attrId, const chunkId cId);
	// For better performance, implement the function in an inherit class.
	// The function provided by default extracts ID from ChunkDesc 
	// and re-generate chunkDesc for it.
	inline pChunk makeChunk(pChunkDesc desc);
	// If a chunkBit is setted, array makes a chunk accordingly.
	void makeChunks(const attributeId attrId, const bitmap& input);

	inline pChunk insertChunk(pChunk inputChunk);
	template <class _Iter>
	void insertChunk(const attributeId attrId, _Iter begin, _Iter end)
	{
		assert(this->attrChunkBitmaps_[attrId] != nullptr);
		for (; begin != end; ++begin)
		{
			auto id = (*begin)->getId();
			this->chunks_[attrId].insert(chunkPair(id, *begin));		// 나머지 chunk들도 모두 만들어주어야 하는 것 아닌가? 왜냐면 chunk bitmap을 set하니까....
			this->setChunkExist(attrId, id);
		}
	}
	inline void setChunkExist(const attributeId attrId, const chunkId cId)
	{
		this->globalChunkBitmap_->setExist(cId);
		this->attrChunkBitmaps_[attrId]->setExist(cId);
	}
	inline void setChunkNull(const attributeId attrId, const chunkId cId)
	{
		this->attrChunkBitmaps_[attrId]->setNull(cId);
		for (auto b : *this->desc_->attrDescs_)
		{
			if (this->attrChunkBitmaps_[b->id_]->isExist(cId))
			{
				return;
			}
		}
		this->globalChunkBitmap_->setNull(cId);
	}
	virtual void freeChunk(const attributeId attrId, const chunkId cId);

	cpBitmap getChunkBitmap() const;
	void copyChunkBitmap(cpBitmap chunkBitmap);
	void replaceChunkBitmap(pBitmap chunkBitmap);
	void mergeChunkBitmap(cpBitmap chunkBitmap);		// TODO::pullout the method from array -> bitmap, andMerge, orMerge.... or operators

	void copyAttrChunkBitmap(const attributeId attrId, cpBitmap chunkBitmap, bool globalUpdated = false);
	void replaceAttrChunkBitmap(const attributeId attrId, pBitmap chunkBitmap, bool globalUpdated = false);
	void mergeAttrChunkBitmap(const attributeId attrId, cpBitmap chunkBitmap, bool globalUpdated = false);

	void deleteAttribute(const attributeId attrId);

public:
	void print();

public:
	void shallowChunkCopy(array& inArr, const bool takeOwnership = false);

protected:
	pArrayDesc desc_;
	multiAttrChunkContainer chunks_;		// std::map<attributeId, chunkContainer>;
	chunkFactories cFactories_;				// std::vector<pChunkFactory>;

private:
	bitmapContainer attrChunkBitmaps_;		// std::map<attributeId, pBitmap>;
	pBitmap globalChunkBitmap_;				// Be initialized to false by default
};
}	// core
}	// msdb
#endif		// _MSDB_ARRAY_H_