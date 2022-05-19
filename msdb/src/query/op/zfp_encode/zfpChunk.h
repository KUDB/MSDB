#pragma once
#ifndef _MSDB_LZWCHUNK_H_
#define _MSDB_LZWCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>
#include <zfp/zfp.h>

namespace msdb
{
namespace core
{
template <typename Ty_>
class zfpBlock : public flattenBlock<Ty_>
{
public:
	zfpBlock(pBlockDesc desc)
		: flattenBlock<Ty_>(desc)
	{

	}
	virtual ~zfpBlock()
	{

	}

public:
	virtual void serialize(bstream& os) override;
	virtual void deserialize(bstream& is) override;

	virtual void serialize(std::stringstream& os);
};

template <typename Ty_>
class zfpChunk : public flattenChunk<Ty_>
{
public:
	using zfpCodeType = std::uint16_t;

public:
	zfpChunk(pChunkDesc desc)
		: flattenChunk<Ty_>(desc)
	{

	}
	virtual ~zfpChunk()
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
			auto blockObj = std::make_shared<zfpBlock<Ty_>>(desc);
			this->insertBlock(blockObj);
		}

		return this->blocks_[bId];
	}

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;
};
//////////////////////////////
// zfpChunkFactory
//
// To make concreteType of zfpChunk
//
template <typename Ty_>
class zfpChunkFactory : public chunkFactory
{
public:
	zfpChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for zfpChunkFacotry
//
class zfpChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	zfpChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<zfpChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// zfpChunkType
//
class zfpChunkType : public chunkType
{
public:
	zfpChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "zfpChunk";
	}
};

template <typename Ty_>
zfp_type getZFPType()
{
	if (typeid(Ty_) == typeid(bool) ||
		typeid(Ty_) == typeid(char) ||
		typeid(Ty_) == typeid(int8_t) ||
		typeid(Ty_) == typeid(uint8_t) ||
		typeid(Ty_) == typeid(int16_t) ||
		typeid(Ty_) == typeid(uint16_t) ||
		typeid(Ty_) == typeid(int32_t) ||
		typeid(Ty_) == typeid(uint32_t))
	{
		return zfp_type_int32;
	}

	if (typeid(Ty_) == typeid(int64_t) ||
		typeid(Ty_) == typeid(uint64_t))
	{
		return zfp_type_int64;
	}

	if (typeid(Ty_) == typeid(float))
	{
		return zfp_type_float;
	}

	if (typeid(Ty_) == typeid(double))
	{
		return zfp_type_double;
	}

	return zfp_type_none;
}
}		// core
}		// msdb

#include "zfpChunk.hpp"
#endif	// _MSDB_LZWCHUNK_H_