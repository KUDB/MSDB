﻿#include <pch.h>
#include <array/blockBuffer.h>

namespace msdb
{
namespace core
{
blockBuffer::blockBuffer(void* data, const bufferSize size)
	: chunkBuffer()
{
	this->refChunkBufferWithoutOwnership(data, size);
}

blockBuffer::~blockBuffer()
{
}

// WARNING:: data is deleted when the chunkBuffer is disappear in a destructor.
void blockBuffer::refChunkBufferWithoutOwnership(void* data, const bufferSize size)
{
	this->isOwned_ = false;
	this->data_ = data;
	this->bodySize_ = size;
}
flattenBlockBuffer::flattenBlockBuffer(void* data, const bufferSize size)
	: blockBuffer(data, size)
{
}
flattenBlockBuffer::~flattenBlockBuffer()
{
}
}		// core
}		// msdb