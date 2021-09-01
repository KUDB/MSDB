#pragma once
#ifndef _MSDB_BUFFER_H_
#define _MSDB_BUFFER_H_

#include <pch.h>

namespace msdb
{
namespace core
{
using bufferSize = unsigned long long;
extern const bufferSize INVALID_BUFFER_SIZE;

class buffer;
using pBuffer = std::shared_ptr<buffer>;

// abstrct class for binary data holding
class buffer : public std::enable_shared_from_this<buffer>
{
public:
	buffer();
	virtual ~buffer();

public:
	virtual void* getData() = 0;								// writable data pointer
	virtual void const* getReadData() const = 0;				// read-only data pointer

	virtual bufferSize size() const = 0;						// buffer size

	virtual void bufferAlloc(bufferSize size) = 0;
	virtual void realloc(bufferSize size) = 0;
	virtual void copy(void* data, bufferSize size) = 0;			// copy input data
	virtual void copy(void* data, bufferSize offset, bufferSize size) = 0;
	virtual void ref(pBuffer refBuffer, bufferSize size) = 0;
	virtual void free();										// free binary data

	inline bool isAllocated()
	{
		if (this->isAllocated_)
			return true;
		return false;
	}

	//virtual bool pin() const = 0;
	//virtual void unPin() const = 0;

protected:
	bufferSize bodySize_;
	bool isAllocated_;
};
}		// core
}		// msdb
#endif	// _MSDB_BUFFER_H_
