#include <pch.h>
#include <array/monoChunkBuffer.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
monoChunkBuffer::monoChunkBuffer()
{

}

monoChunkBuffer::~monoChunkBuffer()
{
}

bool monoChunkBuffer::isCompressed() const
{
	return false;
}
}		// core
}		// msdb
