#include <pch.h>
#include <array/blockBuffer.h>

namespace msdb
{
namespace core
{
blockBuffer::blockBuffer(void* data, bufferSize size)
	: chunkBuffer()
{
	this->refChunkBufferWithoutOwnership(data, size);
}

blockBuffer::~blockBuffer()
{
}

// WARNING:: data is deleted when the chunkBuffer is disappear in a destructor.
void blockBuffer::refChunkBufferWithoutOwnership(void* data, bufferSize size)
{
	this->isAllocated_ = false;
	this->data_ = data;
	this->bodySize_ = size;
}
memBlockBuffer::memBlockBuffer(void* data, bufferSize size)
	: blockBuffer(data, size)
{
}
memBlockBuffer::~memBlockBuffer()
{
}
}		// core
}		// msdb