#pragma once
#include <op/zfp_encode/zfpChunk.h>
#include "zfp.h"

namespace msdb
{
namespace core
{
template<typename Ty_>
void zfpBlock<Ty_>::serialize(bstream& os)
{

}

template<typename Ty_>
void zfpBlock<Ty_>::deserialize(bstream& is)
{

}

//////////////////////////////
// zfpChunkFactory
//
template <typename Ty_>
pChunk zfpChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<zfpChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core
