#pragma once

#include <compression/spihtChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// spihtChunkFactory
//
template <typename Ty_>
pChunk spihtChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<spihtChunk<Ty_>>(cDesc);
}
}		// core
}		// msdb