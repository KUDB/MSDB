#include <pch.h>
#include <array/chunkBuffer.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
chunkBuffer::chunkBuffer()
	: buffer(), data_(nullptr), refBuffer_(nullptr)
{

}

chunkBuffer::~chunkBuffer()
{
	this->free();
}

void* chunkBuffer::getData()
{
	return this->data_;
}

void const* chunkBuffer::getReadData() const
{
	return this->data_;
}

bufferSize msdb::core::chunkBuffer::size() const
{
	return this->bodySize_;
}

void chunkBuffer::bufferAlloc(bufferSize size)
{
	assert(size > 0);

	if(this->data_ != nullptr)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL));
	}
	
	this->isAllocated_ = true;
	this->data_ = new char[size]();
	this->bodySize_ = size;
}

void chunkBuffer::realloc(bufferSize size)
{
	assert(size > 0);

	void* re = new char[size]();
	memcpy(re, this->data_, std::min(size, this->bodySize_));
	this->free();

	this->isAllocated_ = true;
	this->data_ = re;
	this->bodySize_ = size;
}

void chunkBuffer::copy(void* data, bufferSize size)
{
	assert(size <= this->bodySize_);
	//this->free();
	//this->isAllocated_ = true;
	//this->data_ = new char[size];
	memcpy(this->data_, data, size);
	//this->bodySize_ = size;
}

void chunkBuffer::copy(void* data, bufferSize offset, bufferSize size)
{
	assert(offset + size <= this->bodySize_);
	memcpy((char*)this->data_ + offset, data, size);
}

void chunkBuffer::ref(pBuffer refBuffer, bufferSize size)
{
	assert(size >= refBuffer->size());

	auto refChunkBuffer = std::static_pointer_cast<chunkBuffer>(refBuffer);
	this->isAllocated_ = false;
	this->data_ = refBuffer->getData();
	this->bodySize_ = size; 

	if(refChunkBuffer->getRefBuffer())
	{
		this->refBuffer_ = refChunkBuffer->getRefBuffer();
	}else
	{
		this->refBuffer_ = refChunkBuffer;
	}
}

void chunkBuffer::free()
{
	if(this->isAllocated_ && this->data_ != nullptr)
	{
		delete[] this->data_;
		this->isAllocated_ = false;
		this->bodySize_ = 0;
		this->data_ = nullptr;
		this->refBuffer_ = nullptr;
	}
}

pChunkBuffer chunkBuffer::getRefBuffer()
{
	return this->refBuffer_;
}

//void chunkBuffer::compress(CompressionMethod cm)
//{
//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_CHUNK_CANNOT_COMPRESS));
//}
//
//void chunkBuffer::uncompress()
//{
//	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_CHUNK_CANNOT_UNCOMPRESS));
//}
}		// core
}		// msdb