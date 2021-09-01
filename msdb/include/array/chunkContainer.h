#pragma once
#ifndef _MSDB_CHUNKCONTAINER_H_
#define _MSDB_CHUNKCONTAINER_H_

#include <pch.h>
#include <array/chunk.h>

namespace msdb
{
namespace core
{
using chunkContainer = std::map<chunkId, pChunk>;
using chunkPair = std::pair<chunkId, pChunk>;
}		// core
}		// msdb
#endif	// _MSDB_CHUNKCONTAINER_H_
