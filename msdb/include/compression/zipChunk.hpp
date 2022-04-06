#pragma once
#include <compression/zipChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// zipChunkFactory
//
template<typename Ty_>
inline pChunk zipChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return return std::make_shared<zipChunk<Ty_>>(cDesc);
}
}
}