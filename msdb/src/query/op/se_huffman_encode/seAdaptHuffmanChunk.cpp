#include <pch.h>
#include <op/se_huffman_encode/seAdaptHuffmanChunk.h>
#include <op/se_huffman_encode/seHuffmanBlock.h>
#include <util/ioutil.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// seAdaptHuffmanChunkType
//
seAdaptHuffmanChunkType::seAdaptHuffmanChunkType(const dataType& type)
	: chunkType(buildFactory(seAdaptHuffmanChunkFactoryBuilder(), type))
{
}

//REGISTER_CHUNK_FACTORY("seAdaptHuffmanChunk", seAdaptHuffmanChunkFactoryBuilder())

}		// core
}		// msdb