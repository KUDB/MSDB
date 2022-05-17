#include <pch.h>
#include <array/flattenChunkBuffer.h>

namespace msdb
{
namespace core
{
flattenChunkBuffer::flattenChunkBuffer()
	: buffer(), data_(nullptr)
{
}

flattenChunkBuffer::~flattenChunkBuffer()
{
	this->free();
}

void* flattenChunkBuffer::getData()
{
	return this->data_;
}

void const* flattenChunkBuffer::getReadData() const
{
	return this->data_;
}

bufferSize flattenChunkBuffer::size() const
{
	return this->bodySize_;
}

void flattenChunkBuffer::bufferAlloc(bufferSize size)
{
	assert(size > 0);

	if (this->isAllocated())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL));
	}

	try
	{
		this->data_ = new char[size]();
		this->bodySize_ = size;
		this->isOwned_ = true;
	}
	catch (const std::bad_alloc& e)
	{
		this->data_ = nullptr;
		this->isOwned_ = false;
		this->bodySize_ = 0;

		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL, e.what()));
	}
}

void flattenChunkBuffer::realloc(bufferSize size)
{
	assert(size > 0);

	try
	{
		void* re = new char[size]();
		memcpy(re, this->data_, std::min(size, this->bodySize_));
		this->free();

		this->isOwned_ = true;
		this->data_ = re;
		this->bodySize_ = size;
	}
	catch (const std::bad_alloc& e)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL, e.what()));
	}
}

void flattenChunkBuffer::copy(void* data, bufferSize size)
{
	try
	{
		this->data_ = new char[size]();
		memcpy(this->data_, data, size);
		this->isOwned_ = true;
		this->bodySize_ = size;
	}
	catch (const std::bad_alloc& e)
	{
		this->data_ = nullptr;
		this->isOwned_ = false;
		this->bodySize_ = 0;

		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL, e.what()));
	}
}

void flattenChunkBuffer::refChunkBufferWithoutOwnership(void* data, bufferSize size)
{
	this->free();
	this->isOwned_ = false;
	this->data_ = data;
	this->bodySize_ = size;
}

void flattenChunkBuffer::free()
{
	if(this->isOwned_ && this->data_ != nullptr)
	{
		delete[] this->data_;
		this->isOwned_ = false;
		this->bodySize_ = 0;
		this->data_ = nullptr;
		//this->blocks_.clear();
	}
}
}		// core
}		// msdb