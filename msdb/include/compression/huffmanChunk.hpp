#pragma once
#include <compression/huffmanChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// huffmanChunkFactory
//
template <typename Ty_>
pChunk huffmanChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<huffmanChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core