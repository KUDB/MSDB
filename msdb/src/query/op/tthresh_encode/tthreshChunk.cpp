#include <pch.h>
#include "tthreshChunk.h"

namespace msdb
{
namespace core
{
//////////////////////////////
// tthreshChunkType
//
tthreshChunkType::tthreshChunkType(const dataType& type)
	: chunkType(buildFactory(tthreshChunkFactoryBuilder(), type))
{
}
}		// core
}		// msdb