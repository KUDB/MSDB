﻿#pragma once
#include <op/lzw_encode/lzwChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// lzwChunkFactory
//
template <typename Ty_>
pChunk lzwChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<lzwChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core