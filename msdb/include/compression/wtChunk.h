#pragma once
#ifndef _MSDB_WT_CHUNK_H_
#define _MSDB_WT_CHUNK_H_

#include <pch.h>
#include <array/monoChunk.h>

#define _STR_PARAM_WAVELET_LEVEL_	"wavelet_level"

namespace msdb
{
namespace core
{
//class wtChunk;
//using pWtChunk = std::shared_ptr<wtChunk>;

// TODO:: inherit blocked chunk
template <typename Ty_>
class wtChunk : public monoChunk<Ty_>
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
	inline size_t getLevel()
	{
		return this->level_;
	}
	//chunkId getBandId();
	//chunkId getSourceChunkId();

	inline void setLevel(size_t level)
	{
		this->level_ = level;
	}
	//void setBandId(chunkId bid);
	//void setSourceChunkId(chunkId cid);

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
	//chunkId bandId_;
	//chunkId sourceChunkId_;
};

//////////////////////////////
// wtChunkFactory
//
// To make concreteType of wtChunk
//
template <typename Ty_>
class wtChunkFactory : public chunkFactory
{
public:
	wtChunkFactory()
		: chunkFactory()
	{}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc);
};

//////////////////////////////
// Factory constructor for wtChunkFacotry
//
class wtChunkFactoryBuilder : public chunkFactoryBuilder
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
class wtChunkType : public chunkType
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

#include "wtChunk.hpp"

#endif	// _MSDB_WT_CHUNK_H_