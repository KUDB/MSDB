#include <pch.h>
#include <array/memChunkBuffer.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
memChunkBuffer::memChunkBuffer()
{

}

memChunkBuffer::~memChunkBuffer()
{
}

bool memChunkBuffer::isCompressed() const
{
	return false;
}
}		// core
}		// msdb
