#pragma once
#ifndef _MSDB_BLOCKBUFFER_H_
#define _MSDB_BLOCKBUFFER_H_

#include <pch.h>
#include <array/chunkBuffer.h>

namespace msdb
{
namespace core
{
class blockBuffer;
using pBlockBuffer = std::shared_ptr<blockBuffer>;

// Note: block buffer only holds a pointer to a chunk buffer memory.
class blockBuffer : public chunkBuffer
{
public:
	blockBuffer(void* data, const bufferSize size);
	virtual ~blockBuffer();

public:
	void refChunkBufferWithoutOwnership(void* data, const bufferSize size);
};

class flattenBlockBuffer : public blockBuffer
{
public:
	flattenBlockBuffer(void* data, const bufferSize size);
	virtual ~flattenBlockBuffer();
};
}		// core
}		// msdb
#endif	// _MSDB_BLOCKBUFFER_H_
