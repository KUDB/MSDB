#pragma once
#ifndef _MSDB_FLATTENCHUNK_H_
#define _MSDB_FLATTENCHUNK_H_

#include <pch.h>
#include <array/chunkIterator.h>
#include <array/monoChunkItemIterator.h>
#include <util/coordinate.h>
#include <array/block.h>
#include <array/chunkIterator.h>
#include <array/chunkItemIterator.h>
#include <io/bitstream.h>
#include <util/dataType.h>
#include <array/chunkType.h>
#include <array/chunkFactory.h>

namespace msdb
{
namespace core
{
class blockIterator;

template <typename Ty_>
class flattenBlock : public block
{
public:
	flattenBlock(pBlockDesc desc);
	virtual ~flattenBlock();

	//////////////////////////////
	// Item Iterators
	//////////////////////////////
public:
	virtual pBlockItemIterator getItemIterator();
	virtual const pBlockItemIterator getItemIterator() const;
	virtual pBlockItemRangeIterator getItemRangeIterator(const range& range);
	virtual const pBlockItemRangeIterator getItemRangeIterator(const range& range) const;

	virtual vpItemIterator getValueIterator();
	virtual vpItemIterator getValueRangeIterator(const range& range);

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	virtual void refChunkBufferWithoutOwnership(void* data, bufferSize size) override;

	//////////////////////////////
	// Serializable
	//////////////////////////////
public:
	virtual void serialize(bstream& os) override;
	virtual void deserialize(bstream& is) override;
};

template <typename Ty_>
class flattenChunk : public chunk
{
public:
	using size_type = chunk::size_type;

public:
	flattenChunk(pChunkDesc desc);
	virtual ~flattenChunk();

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
	virtual pConstBlockIterator getBlockIterator(
		const iterateMode itMode = iterateMode::ALL) const override;

	//////////////////////////////
	// Setter
	//////////////////////////////
	

//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	virtual pChunkItemIterator getItemIterator();
	virtual pChunkItemRangeIterator getItemRangeIterator(const range& range);

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
								const range& range,
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

//////////////////////////////
// flattenChunkFactory
//
// To make concreteType of flattenChunk
//
template <typename Ty_>
class flattenChunkFactory : public chunkFactory
{
public:
	flattenChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for flattenChunkFacotry
//
class flattenChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	flattenChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<flattenChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// flattenChunkType
//
class flattenChunkType : public chunkType
{
public:
	flattenChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "flattenChunk";
	}
};
}		// core
}		// msdb

#include "flattenChunk.hpp"

#endif	// _MSDB_FLATTENCHUNK_H_