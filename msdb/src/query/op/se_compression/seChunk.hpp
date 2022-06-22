#pragma once
#include "seChunk.h"

namespace msdb
{
namespace core
{
//////////////////////////////
// seChunkFactory
//
template <typename Ty_>
pChunk seChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<seChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core