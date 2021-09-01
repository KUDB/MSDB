#pragma once
#ifndef _MSDB_CHUNK_H_
#define _MSDB_CHUNK_H_

#include <pch.h>
#include <io/bitstream.h>
#include <io/serializable.h>

#include <array/chunkDesc.h>
#include <array/blockIterator.h>

namespace msdb
{
namespace core
{
class chunk;
using pChunk = std::shared_ptr<chunk>;

class chunk : public std::enable_shared_from_this<chunk>, public serializable
{
public:
	using size_type = size_t;

public:
	chunk(pChunkDesc desc);
	virtual ~chunk();

//////////////////////////////
// Desc
//////////////////////////////
public:
	chunkId getId() const;
	void setId(chunkId id);	// chunk id can be chnaged in query processing
	const pChunkDesc getDesc() const;
	void setChunkDesc(const pChunkDesc inDesc);
	size_type getDSize();
	size_type numCells();
	coor getChunkCoor();
	coorRange getChunkRange();
	void flush();

protected:
	pChunkDesc desc_;		// chunk desc

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	void freeBuffer();
	virtual void makeBuffer() = 0;
	virtual void referenceBufferToBlock(const blockId bId) = 0;
	virtual void referenceAllBufferToBlock();

public:
	virtual void bufferAlloc();
	virtual void bufferAlloc(bufferSize size);
	virtual void bufferCopy(void* data, bufferSize size);
	virtual void bufferCopy(pChunk source);
	virtual void bufferCopy(pBlock source);
	//virtual void bufferRef(void* data, bufferSize size);
	virtual void bufferRef(pChunk source);
	bool isMaterialized() const;

protected:
	pChunkBuffer getBuffer();

protected:
	pChunkBuffer cached_;	// hold materialized chunk

//////////////////////////////
// Blocks
//////////////////////////////
public:
	virtual pBlock makeBlock(const blockId bId) = 0;
	virtual void makeBlocks(const bitmap& blockBitmap);
	virtual void makeBlocks();
	virtual void makeAllBlocks();
	virtual void insertBlock(pBlock inBlock) = 0;
	virtual void freeBlock(const blockId bid) = 0;

	// mSize and mOffset size are not setted in the output of getBlockDesc function
	virtual pBlockDesc getBlockDesc(const blockId bId);
	size_t getBlockCapacity();
	virtual pBlock getBlock(const blockId bId) = 0;
	//virtual blockId getBlockId(pBlockDesc cDesc) = 0;
	//virtual blockId getBlockIdFromItemCoor(coor& itemCoor) = 0;
	//virtual blockId getBlockIdFromBlockCoor(coor& blockCoor) = 0;
	//virtual coor itemCoorToBlockCoor(coor& itemCoor) = 0;
	virtual coor getBlockCoor(const blockId bId);
	virtual pBlockIterator getBlockIterator(
		const iterateMode itMode = iterateMode::ALL) = 0;
	void copyBlockBitmap(cpBitmap blockBitmap);
	void replaceBlockBitmap(pBitmap blockBitmap);
	void mergeBlockBitmap(pBitmap blockBitmap);
	pBitmap getBlockBitmap();
	//cpBitmap getBlockBitmap() const;

protected:
	size_type blockCapacity_;
	pBitmap blockBitmap_;		// Be initialized to false by default

//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	//virtual pChunkItemIterator getItemIterator() = 0;
	//virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range) = 0;

//////////////////////////////
// Print
//////////////////////////////
public:
	void print();

protected:
	template <class Ty_>
	void printImp()
	{

	}
	template<>
	void printImp<char>()
	{

	}

//////////////////////////////
// Serializable
//////////////////////////////
protected:
	class chunkHeader : public serialHeader
	{
	public:
		static const char chunk_header_version = 1;

	public:
		virtual void serialize(std::ostream& os) override
		{
			//std::cout << "Chunk Header serialize" << std::endl;
			//std::cout << this->version_ << ", " << this->bodySize_ << std::endl;

			os.write((char*)(&this->version_), sizeof(this->version_));
			os.write((char*)(&this->bodySize_), sizeof(this->bodySize_));
		}
		virtual void deserialize(std::istream& is) override
		{
			//std::cout << "Chunk Header deserialize" << std::endl;

			is.read((char*)(&this->version_), sizeof(this->version_));
			is.read((char*)(&this->bodySize_), sizeof(this->bodySize_));
			//BOOST_LOG_TRIVIAL(info) << "BODY: " << this->bodySize_;
			//std::cout << this->version_ << ", " << this->bodySize_ << std::endl;
		}
	};

public:
	virtual void updateToHeader() override;
	virtual void updateFromHeader() override;
};
}		// core
}		// msdb
#endif	// _MSDB_CHUNK_H_