#pragma once
#include <compression/seHuffmanChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// seHuffmanChunkFactory
//
template <typename Ty_>
pChunk seHuffmanChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<seHuffmanChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core