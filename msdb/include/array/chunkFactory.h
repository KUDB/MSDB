#pragma once
#ifndef _MSDB_CHUNKFACTORY_H_
#define _MSDB_CHUNKFACTORY_H_

#include <pch.h>
#include <util/singleton.h>
#include <array/chunk.h>

namespace msdb
{
namespace core
{
#define REGISTER_CHUNK_FACTORY(chunkname, builder)\
	chunkFactoryManager::instance()->registerFactoryBuilder(chunkname, builder);

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

class chunkFactoryBuilder
{
public:
	chunkFactoryBuilder() = default;
};

class chunkFactoryManager : public singleton<chunkFactoryManager>
{
private:
	chunkFactoryManager() = default;
	~chunkFactoryManager();

public:
	void registerFactoryBuilder(const std::string& name, const chunkFactoryBuilder& builder);
	
	// If it dose not have matched name of chunkType, the function throws an out_of_range exception.
	chunkFactoryBuilder getFactoryBuilder(const std::string& name);

protected:
	friend singleton<chunkFactoryManager>;

private:
	std::map<std::string, chunkFactoryBuilder> builders_;
};
}		// core
}		// msdb

#endif _MSDB_CHUNKFACTORY_H_