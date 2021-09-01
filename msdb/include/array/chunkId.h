#pragma once
#ifndef _MSDB_CHUNK_ID_H_
#define _MSDB_CHUNK_ID_H_

#include <cstdint>

namespace msdb
{
namespace core
{
typedef uint32_t chunkId;

const chunkId INVALID_CHUNK_ID = static_cast<chunkId>(~0);
}		// core
}		// msdb
#endif	// _MSDB_CHUNK_ID_H_