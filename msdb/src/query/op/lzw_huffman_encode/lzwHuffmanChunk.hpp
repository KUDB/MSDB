#pragma once
#include <op/lzw_huffman_encode/lzwHuffmanChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// lzwHuffmanChunkFactory
//
template <typename Ty_>
pChunk lzwHuffmanChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<lzwHuffmanChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core