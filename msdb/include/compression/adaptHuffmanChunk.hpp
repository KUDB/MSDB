#pragma once
#include <compression/adaptHuffmanChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// adaptHuffmanChunkFactory
//
template <typename Ty_>
pChunk adaptHuffmanChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<adaptHuffmanChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core