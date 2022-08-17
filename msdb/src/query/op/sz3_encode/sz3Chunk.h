#pragma once
#ifndef _MSDB_SZ3CHUNK_H_
#define _MSDB_SZ3CHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <op/sz3_encode/sz3Chunk.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class sz3Block : public flattenBlock<Ty_>
{
public:
	sz3Block(pBlockDesc desc)
		: flattenBlock<Ty_>(desc)
	{

	}
	virtual ~sz3Block()
	{

	}

public:
	// Serializable interface is not used in tthreshChunk
	virtual void serialize(bstream& os) override;
	virtual void deserialize(bstream& is) override;

	void serialize(std::ostream& os);
	void deserialize(std::istream& is);
};

template <typename Ty_>
class sz3Chunk : public flattenChunk<Ty_>
{
public:
	sz3Chunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}
	virtual ~sz3Chunk()
	{

	}

public:
	virtual pBlock makeBlock(const blockId bId) override
	{
		assert(this->blockCapacity_ > bId);
		if (this->blocks_[bId] == nullptr)
		{
			// Make new one
			auto desc = this->getBlockDesc(bId);
			auto blockObj = std::make_shared<sz3Block<Ty_>>(desc);
			this->insertBlock(blockObj);
		}

		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;
};

//////////////////////////////
// sz3ChunkFactory
//
// To make concreteType of sz3Chunk
//
template <typename Ty_>
class sz3ChunkFactory : public chunkFactory
{
public:
	sz3ChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};		// sz3ChunkFactory

//////////////////////////////
// Factory constructor for sz3ChunkFacotry
//
class sz3ChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	sz3ChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<sz3ChunkFactory<Ty_>>();
	}
};		// sz3ChunkFactoryBuilder

//////////////////////////////
// sz3ChunkType
//
class sz3ChunkType : public chunkType
{
public:
	sz3ChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "sz3Chunk";
	}
};		// sz3ChunkType
}		// core
}		// msdb

#include "sz3Chunk.hpp"

#endif	// _MSDB_SZ3CHUNK_H_