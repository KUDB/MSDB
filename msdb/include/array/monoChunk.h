#pragma once
#ifndef _MSDB_MEMCHUNK_H_
#define _MSDB_MEMCHUNK_H_

#include <pch.h>
#include <array/chunk.h>
#include <array/chunkIterator.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class monoChunk : public chunk
{
public:
	monoChunk(pChunkDesc desc);
	virtual ~monoChunk();

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	virtual void makeBuffer();
	virtual void referenceBufferToBlock(const blockId bId);

//////////////////////////////
// Blocks
//////////////////////////////
public:
	virtual pBlock makeBlock(const blockId bId) override;
	virtual void insertBlock(pBlock inBlock) override;
	virtual pBlock getBlock(const blockId bId) override;
	virtual void freeBlock(const blockId bid) override;

	//virtual blockId getBlockId(pBlockDesc cDesc) override;
	//virtual blockId getBlockIdFromItemCoor(coor& itemCoor) override;
	//virtual blockId getBlockIdFromBlockCoor(coor& blockCoor) override;
	//virtual coor itemCoorToBlockCoor(coor& itemCoor) override;
	virtual coor blockId2blockCoor(const blockId bId);
	virtual pBlockIterator getBlockIterator(
		const iterateMode itMode = iterateMode::ALL) override;

protected:
	//blockContainer blocks_;
	pBlock block_;	// monoChunk has single block.

//////////////////////////////
// Item Iterators
//////////////////////////////
public:
	//virtual pChunkItemIterator getItemIterator();
	//virtual pChunkItemRangeIterator getItemRangeIterator(const range& range);

//////////////////////////////
// Serializable
//////////////////////////////
public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serialize(bstream& bs)
	{
		//bs << setw(sizeof(Ty_) * CHAR_BIT);
		//auto it = this->getItemIterator();

		//while (!it->isEnd())
		//{
		//	bs << (**it).get<Ty_>();
		//	++(*it);
		//}
	}
	template<class Ty_>
	void deserialize(bstream& bs)
	{
		//bs >> setw(sizeof(Ty_) * CHAR_BIT);
		//auto it = this->getItemIterator();

		//while (!it->isEnd())
		//{
		//	Ty_ value;
		//	bs >> value;
		//	(**it).set<Ty_>(value);
		//	++(*it);
		//}
	}
};

//////////////////////////////
// monoChunkFactory
//
// To make concreteType of monoChunk
//
template <typename Ty_>
class monoChunkFactory : public chunkFactory
{
public:
	monoChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for monoChunkFacotry
//
class monoChunkFactoryBuilder
{
public:
	monoChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<monoChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// monoChunkType
//
class monoChunkType : public chunkType
{
public:
	monoChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "monoChunk";
	}
};
}		// core
}		// msdb

#include "monoChunk.hpp"

#endif	// _MSDB_MEMCHUNK_H_