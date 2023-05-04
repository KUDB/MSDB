#include "seChunk.h"

namespace msdb
{
namespace core
{
//////////////////////////////
// seChunkType
//
seChunkType::seChunkType(const dataType& type)
	: chunkType(buildFactory(seChunkFactoryBuilder(), type))
{
}

//REGISTER_CHUNK_FACTORY("seChunk", seChunkFactoryBuilder())
}		// core
}		// msdb