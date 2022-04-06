#pragma once
#ifndef _MSDB_NESTEDCHUNK_H_
#define _MSDB_NESTEDCHUNK_H_

#include <pch.h>
#include <array/chunkIterator.h>
#include <array/chunkFactory.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class nestedChunk : public chunk
{
public:
	using size_type = chunk::size_type;
	using level_type = int16_t;

public :
	nestedChunk(pChunkDesc desc);
	virtual ~nestedChunk();

//////////////////////////////
// Buffer
//////////////////////////////
protected:
	virtual void makeBuffer();
	virtual void referenceBufferToBlock(const blockId bId);

public:
	virtual pBlock makeBlock(const blockId bId) override;
	virtual void insertBlock(pBlock inBlock) override;
	virtual pBlock getBlock(const blockId bId) override;
	virtual void freeBlock(const blockId bid) override;
	virtual coor blockId2blockCoor(const blockId bId);
	virtual pBlockIterator getBlockIterator(
		const iterateMode itMode = iterateMode::ALL) override;

public:
//////////////////////////////
// Serializable
//////////////////////////////
public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

protected:
	// Convert Level with External Block ID to Internal Block ID
	blockId exBlockId2InBlockId(const level_type& level, const blockId& exId);
	dimension getBlockSpaceByLevel(const level_type& level);		// TODO::method for chunkDesc ??

private:
	level_type _level;
	blockContainer blocks_;
};		// nestedChunk

//////////////////////////////
// nestedChunkFactory
//
// To make concreteType of nestedChunk
//
template <typename Ty_>
class nestedChunkFactory : public chunkFactory
{
public:
	nestedArrayChunkFactory()
		: chunkFactory()
	{

	}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};		// nestedChunkFactory

//////////////////////////////
// Factory constructor for nestedChunkFactory
//
class nestedChunkFactoryBuilder
{
public:
	nestedChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(concreteTy<Ty_> type)
	{
		return std::make_shared<nestedChunkFactory<Ty_>>();
	}
};		// nestedChunkFactoryBuilder

//////////////////////////////
// nestedChunk ChunkType
//
class nestedChunkType : public chunkType
{
public:
	nestedChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "nestedChunk";
	}
};
}		// core
}		// msdb

#include "nestedChunk.hpp"

#endif	_MSDB_NESTEDCHUNK_H_