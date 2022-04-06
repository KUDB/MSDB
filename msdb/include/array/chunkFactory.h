#pragma once
#ifndef _MSDB_CHUNKFACTORY_H_
#define _MSDB_CHUNKFACTORY_H_

#include <pch.h>
#include <array/chunk.h>

namespace msdb
{
namespace core
{
class chunkFactory;
using pChunkFactory = std::shared_ptr<chunkFactory>;
using chunkFactories = std::vector<pChunkFactory>;

class chunkFactory
{
public:
	chunkFactory();

public:
	inline pChunk requestNewChunk(pChunkDesc cDesc)
	{
		return this->makeChunk(cDesc);
	}

protected:
	virtual pChunk makeChunk(pChunkDesc cDesc) = 0;
};		// chunkFactory
}		// core
}		// msdb

#endif _MSDB_CHUNKFACTORY_H_