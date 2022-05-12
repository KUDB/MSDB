#pragma once
#include <op/wavelet_encode/wtChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// wtChunkFactory
//
template<typename Ty_>
inline pChunk wtChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<wtChunk<Ty_>>(cDesc);
}
}		// core
}		// msdb