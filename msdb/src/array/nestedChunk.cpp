#include <pch.h>
#include <array/nestedChunk.h>

namespace msdb
{
namespace core
{
nestedChunkType::nestedChunkType(const dataType& type)
	: chunkType(buildFactory(nestedChunkFactoryBuilder(), type))
{
	//nestedChunkFactoryBuilder ncfc;
	//this->myFactory_ = std::visit(ncfc, type);
}

//REGISTER_CHUNK_FACTORY("nestedChunk", nestedChunkFactoryBuilder())
}		// core
}		// msdb