#pragma once
#include <op/se_huffman_encode/seAdaptHuffmanChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// seAdaptHuffmanChunkFactory
//
template <typename Ty_>
pChunk seAdaptHuffmanChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<seAdaptHuffmanChunk<Ty_>>(cDesc);
}
}		// msdb
}		// core