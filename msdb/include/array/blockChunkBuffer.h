#pragma once
#ifndef _MSDB_BLOCKCHUNKBUFFER_H_
#define _MSDB_BLOCKCHUNKBUFFER_H_

#include <pch.h>
#include <array/buffer.h>
#include <array/block.h>

namespace msdb
{
namespace core
{
class blockChunkBuffer;
using pBlockChunkBuffer = std::shared_ptr<blockChunkBuffer>;

class blockChunkBuffer : public buffer
{
public:
	blockChunkBuffer();
	virtual ~blockChunkBuffer();

public:
	// buffer을(를) 통해 상속됨
	virtual void* getData() override;
	virtual void const* getReadData() const override;

	virtual bufferSize size() const override;

	virtual void bufferAlloc(bufferSize size) override;
	virtual void realloc(bufferSize size) override;
	virtual void copy(void* data, bufferSize size) override;
	virtual void refChunkBufferWithoutOwnership(void* data, bufferSize size);
	virtual void free() override;

protected:
	void* data_;
};
}		// core
}		// msdb
#endif	// _MSDB_BLOCKCHUNKBUFFER_H_