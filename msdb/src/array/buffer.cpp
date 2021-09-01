#include <pch.h>
#include <array/buffer.h>
#include <assert.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
const bufferSize INVALID_BUFFER_SIZE = static_cast<bufferSize>(~0);

buffer::buffer()
	:bodySize_(0), isAllocated_(false)
{
}

buffer::~buffer()
{
}

void buffer::free()
{
	assert(false);
	_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(
		MSDB_EC_LOGIC_ERROR, MSDB_ER_NOT_IMPLEMENTED, "buffer::free()"));
}
}		// core
}		// msdb
