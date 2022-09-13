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

	virtual void bufferAlloc(const bufferSize size) = 0;
	virtual void realloc(const bufferSize size) = 0;
	virtual void copy(void* data, const bufferSize size) = 0;			// copy input data
	virtual void copy(void* data, const bufferSize offset, const bufferSize size) = 0;
	virtual void ref(pBuffer refBuffer, const bufferSize size, const bool takeOwnership = false) = 0;
	virtual void free();										// free binary data

	inline bool isOwned()
	{
		return this->isOwned_;
	}

	//virtual bool pin() const = 0;
	//virtual void unPin() const = 0;

protected:
	bufferSize bodySize_;
	bool isOwned_;
};
}		// core
}		// msdb
#endif	// _MSDB_BUFFER_H_
