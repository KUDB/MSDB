#include <pch_op.h>
#include <op/wavelet_encode/wtChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// wtChunkType
//
wtChunkType::wtChunkType(const dataType& type)
	: chunkType(buildFactory(wtChunkFactoryBuilder(), type))
{
}

//REGISTER_CHUNK_FACTORY("wtChunk", wtChunkFactoryBuilder())
}		// core
}		// msdb