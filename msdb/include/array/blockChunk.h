#pragma once
#ifndef _MSDB_BLOCKEDCHUNK_H_
#define _MSDB_BLOCKEDCHUNK_H_

#include <pch.h>
#include <array/chunkIterator.h>
#include <array/memChunkItemIterator.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
class blockIterator;

class memBlockChunk : public chunk
{
public:
	using size_type = chunk::size_type;

public:
	memBlockChunk(pChunkDesc desc);
	virtual ~memBlockChunk();

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	virtual void makeBuffer() override;
	virtual void referenceBufferToBlock(const blockId bId) override;
	
public:
	//void flush();

//////////////////////////////
// Blocks
//////////////////////////////
public:
	virtual pBlock makeBlock(const blockId bId) override;
	virtual void insertBlock(pBlock inBlock) override;
	virtual pBlock getBlock(const blockId bId) override;
	virtual void freeBlock(const blockId bid) override;
	//virtual blockId getBlockId(pBlockDesc cDesc) override;
	//virtual blockId getBlockIdFromItemCoor(coor& itemCoor);
	//virtual blockId getBlockIdFromBlockCoor(coor& blockCoor);
	//virtual coor itemCoorToBlockCoor(coor& itemCoor);
	virtual pBlockIterator getBlockIterator(
		const iterateMode itMode = iterateMode::ALL) override;

	//////////////////////////////
	// Setter
	//////////////////////////////
	

//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	virtual pChunkItemIterator getItemIterator();
	virtual pChunkItemRangeIterator getItemRangeIterator(const coorRange& range);

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

protected:
	blockContainer blocks_;
};

class blockChunkItemIterator : public chunkItemIterator
{
public:
	using self_type = blockChunkItemIterator;
	using base_type = chunkItemIterator;

	using coordinate_type = base_type::coordinate_type;
	using size_type = base_type::size_type;
	using dim_type = base_type::dim_type;
	using dim_pointer = base_type::dim_pointer;
	using dim_const_pointer = base_type::dim_const_pointer;
	using dim_reference = base_type::dim_reference;
	using dim_const_reference = base_type::dim_const_reference;
public:
	blockChunkItemIterator(void* data, const eleType eType,
						   const dimension& dims,
						   const dimension& csP,
						   pBlockIterator bItr);

public:
	virtual void next() override;
	virtual void prev() override;

	virtual element getAtDimPos(position_t dimPos) override;
	virtual element getAtSeqPos(position_t seqPos) override;
	virtual element operator*() override;

protected:
	void initBlockItemItr();
	pBlockIterator bItr_;
	pBlockItemIterator curBlockItemItr_;
};

class blockChunkItemRangeIterator : public chunkItemRangeIterator
{
public:
	blockChunkItemRangeIterator(void* data, eleType eType, 
								const dimension& dims,
								const coorRange& range,
								const dimension& csP, 
								pBlockIterator bItr);

public:
	virtual void next() override;
	virtual void prev() override;

	virtual element getAtDimPos(position_t dimPos) override;
	virtual element getAtSeqPos(position_t seqPos) override;
	virtual element operator*() override;

protected:
	pBlockIterator bItr_;
	pBlockItemRangeIterator curBlockItemItr_;
};
}		// core
}		// msdb
#endif	// _MSDB_BLOCKEDCHUNK_H_