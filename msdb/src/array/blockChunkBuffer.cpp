#include <pch.h>
#include <array/blockChunkBuffer.h>

namespace msdb
{
namespace core
{
blockChunkBuffer::blockChunkBuffer()
	: buffer(), data_(nullptr)
{
}

blockChunkBuffer::~blockChunkBuffer()
{
	this->free();
}

void* blockChunkBuffer::getData()
{
	return this->data_;
}

void const* blockChunkBuffer::getReadData() const
{
	return this->data_;
}

bufferSize blockChunkBuffer::size() const
{
	return this->bodySize_;
}

void blockChunkBuffer::bufferAlloc(bufferSize size)
{
	assert(size > 0);

	if (this->isAllocated())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL));
	}

	this->isAllocated_ = true;
	this->data_ = new char[size]();
	this->bodySize_ = size;
	this->isAllocated_ = true;
}

void blockChunkBuffer::realloc(bufferSize size)
{
	assert(size > 0);

	void* re = new char[size]();
	memcpy(re, this->data_, std::min(size, this->bodySize_));
	this->free();

	this->isAllocated_ = true;
	this->data_ = re;
	this->bodySize_ = size;
}

void blockChunkBuffer::copy(void* data, bufferSize size)
{
	this->isAllocated_ = true;
	this->data_ = new char[size]();
	memcpy(this->data_, data, size);
	this->bodySize_ = size;
}

void blockChunkBuffer::refChunkBufferWithoutOwnership(void* data, bufferSize size)
{
	this->free();
	this->isAllocated_ = false;
	this->data_ = data;
	this->bodySize_ = size;

}

void blockChunkBuffer::free()
{
	if(this->isAllocated_ && this->data_ != nullptr)
	{
		delete[] this->data_;
		this->isAllocated_ = false;
		this->bodySize_ = 0;
		this->data_ = nullptr;
		//this->blocks_.clear();
	}
}
}		// core
}		// msdb