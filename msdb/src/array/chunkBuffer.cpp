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

void chunkBuffer::bufferAlloc(const bufferSize size)
{
	assert(size > 0);

	if(this->data_ != nullptr)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_MEMORY_ERROR, MSDB_ER_MEMORY_ALLOC_FAIL));
	}
	
	this->isOwned_ = true;
	this->data_ = new char[size]();
	memset(this->data_, 0, size * sizeof(char));
	this->bodySize_ = size;
}

void chunkBuffer::realloc(const bufferSize size)
{
	assert(size > 0);

	void* re = new char[size]();
	memset(re, 0, size * sizeof(char));
	memcpy(re, this->data_, std::min(size, this->bodySize_));
	this->free();

	this->isOwned_ = true;
	this->data_ = re;
	this->bodySize_ = size;
}

void chunkBuffer::copy(void* data, const bufferSize size)
{
	assert(size <= this->bodySize_);
	//this->free();
	//this->isOwned_ = true;
	//this->data_ = new char[size];
	memcpy(this->data_, data, size);
	//this->bodySize_ = size;
}

void chunkBuffer::copy(void* data, const bufferSize offset, const bufferSize size)
{
	assert(offset + size <= this->bodySize_);
	memcpy((char*)this->data_ + offset, data, size);
}

void chunkBuffer::ref(pBuffer refBuffer, const bufferSize size, const bool takeOwnership)
{
	assert(size >= refBuffer->size());

	auto refChunkBuffer = std::static_pointer_cast<chunkBuffer>(refBuffer);
	this->isOwned_ = takeOwnership;
	this->data_ = refBuffer->getData();
	this->bodySize_ = size; 

	if(refChunkBuffer->getRefBuffer())
	{
		this->refBuffer_ = refChunkBuffer->getRefBuffer();
	}else
	{
		this->refBuffer_ = refChunkBuffer;
	}
	this->refBuffer_->isOwned_ = !takeOwnership;
}

void chunkBuffer::free()
{
	if(this->isOwned_ && this->data_ != nullptr)
	{
		delete[] this->data_;
		this->isOwned_ = false;
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