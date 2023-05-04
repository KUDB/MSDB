#pragma once
#ifndef _MSDB_WT_CHUNK_H_
#define _MSDB_WT_CHUNK_H_

#include <pch_op.h>
#include <encoding/encodingParam.h>
#include <array/monoChunk.h>

namespace msdb
{
namespace core
{
// TODO:: inherit blocked chunk
template <typename Ty_>
class OP_API wtChunk : public monoChunk<Ty_>
{
public:
	wtChunk(pChunkDesc desc)
		: monoChunk<Ty_>(desc), level_(0)
	{
	}
	virtual ~wtChunk()
	{

	}

public:
	dimension getTileSpace(dimension sourceChunkDim)
	{
		dimension tileSpace(sourceChunkDim);
		dimension tileDim(sourceChunkDim);
		tileDim /= pow(2, this->level_ + 1);
		tileSpace /= tileDim;

		return tileSpace;
	}

protected:
	size_t level_;
};

//////////////////////////////
// wtChunkFactory
//
// To make concreteType of wtChunk
//
template <typename Ty_>
class OP_API wtChunkFactory : public chunkFactory
{
public:
	wtChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc)
	{
		return std::make_shared<wtChunk<Ty_>>(cDesc);
	}
};

//////////////////////////////
// Factory constructor for wtChunkFacotry
//
class OP_API wtChunkFactoryBuilder : public chunkFactoryBuilder
{
public:
	wtChunkFactoryBuilder() = default;

public:
	// Visitor
	template <typename Ty_>
	pChunkFactory operator()(const concreteTy<Ty_>& type)
	{
		return std::make_shared<wtChunkFactory<Ty_>>();
	}
};

//////////////////////////////
// wtChunkType
//
class OP_API wtChunkType : public chunkType
{
public:
	wtChunkType(const dataType& type);

public:
	virtual std::string name() override
	{
		return "wtChunk";
	}
};
}		// core
}		// msdb
#endif	// _MSDB_WT_CHUNK_H_