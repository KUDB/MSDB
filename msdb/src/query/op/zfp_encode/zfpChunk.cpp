#include <pch.h>
#include <op/zfp_encode/zfpChunk.h>
#include <util/ioutil.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// zfpChunkType
//
zfpChunkType::zfpChunkType(const dataType& type)
	: chunkType(buildFactory(zfpChunkFactoryBuilder(), type))
{
}
}		// core
}		// msdb