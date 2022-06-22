#pragma once
#ifndef _MSDB_TTHRESH_H_
#define _MSDB_TTHRESH_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class tthreshBlock : public flattenBlock<Ty_>
{
public:
	tthreshBlock(pBlockDesc desc)
		: flattenBlock<Ty_>(desc)
	{

	}

	virtual ~tthreshBlock()
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
class tthreshChunk : public flattenChunk<Ty_>
{
public:
	tthreshChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}

	virtual ~tthreshChunk()
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
			auto blockObj = std::make_shared<tthreshBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
		}

		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

};

//////////////////////////////
// tthreshChunkFactory
//
// To make concreteType of tthreshChunk
//
template <typename Ty_>
class tthreshChunkFactory : public chunkFactory
{
public:
	tthreshChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for tthreshChunkFacotry
//
class tthreshChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	tthreshChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<tthreshChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// tthreshChunkType
//
class tthreshChunkType : public chunkType
{
public:
	tthreshChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "tthreshChunk";
	}
};
}		// core
}		// msdb

#include "tthreshChunk.hpp"
#endif	// _MSDB_TTHRESH_H_