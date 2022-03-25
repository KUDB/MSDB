#pragma once
#ifndef _MSDB_CHUNKFACTORY_H_
#define _MSDB_CHUNKFACTORY_H_

#include <pch.h>
#include <array/chunk.h>

namespace msdb
{
namespace core
{
class chunkFactory
{
public:
	chunkFactory();

public:
	inline pChunk requestNewChunk(const pChunkDesc cDesc)
	{
		return this->makeChunk(cDesc);
	}

protected:
	virtual pChunk makeChunk(const pChunkDesc cDesc) = 0;
};
<<<<<<< Updated upstream
=======

using upChunkFactory = std::unique_ptr<chunkFactory>;
using chunkFactories = std::vector<upChunkFactory>;
>>>>>>> Stashed changes
}		// core
}		// msdb

#endif _MSDB_CHUNKFACTORY_H_