#pragma once
#ifndef _MSDB_CHUNKBUFFER_H_
#define _MSDB_CHUNKBUFFER_H_

#include <pch.h>
#include <array/buffer.h>

namespace msdb
{
namespace core
{
class chunkBuffer;
using pChunkBuffer = std::shared_ptr<chunkBuffer>;

class chunkBuffer : public buffer
{
public:
	chunkBuffer();
	virtual ~chunkBuffer();

public:
	virtual void* getData();
	virtual void const* getReadData() const;

	virtual bufferSize size() const;

	virtual void bufferAlloc(const bufferSize size);
	virtual void realloc(const bufferSize size);
	virtual void copy(void* data, const bufferSize size);
	virtual void copy(void* data, const bufferSize offset, const bufferSize size);
	virtual void ref(pBuffer refBuffer, const bufferSize size, const bool takeOwnership = false);
	virtual void free();

protected:
	pChunkBuffer getRefBuffer();
	
protected:
	void* data_;
	pChunkBuffer refBuffer_;		// Hold a refBuffer to protect it from a garbage collecting.
};
}		// core
}		// msdb
#endif	// _MSDB_CHUNKBUFFER_H_
