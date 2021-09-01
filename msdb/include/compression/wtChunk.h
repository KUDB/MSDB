#pragma once
#ifndef _MSDB_WT_CHUNK_H_
#define _MSDB_WT_CHUNK_H_

#include <pch.h>
#include <array/memChunk.h>

namespace msdb
{
namespace core
{
class wtChunk;
using pWtChunk = std::shared_ptr<wtChunk>;

// TODO:: inherit blocked chunk
class wtChunk : public memChunk
{
public:
	wtChunk(pChunkDesc desc);
	virtual ~wtChunk();

public:
	size_t getLevel();
	//chunkId getBandId();
	//chunkId getSourceChunkId();

	void setLevel(size_t level);
	//void setBandId(chunkId bid);
	//void setSourceChunkId(chunkId cid);

	dimension getTileSpace(dimension sourceChunkDim);

protected:
	size_t level_;
	//chunkId bandId_;
	//chunkId sourceChunkId_;
};
}		// core
}		// msdb
#endif	// _MSDB_WT_CHUNK_H_