#pragma once
#include <compression/compassChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// compassChunkFactory
//
template <typename Ty_>
pChunk compassChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<compassChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core