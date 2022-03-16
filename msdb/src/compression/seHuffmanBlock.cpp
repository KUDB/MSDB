#include <pch.h>
#include <compression/seHuffmanBlock.h>

namespace msdb
{
namespace core
{
seHuffmanBlock::seHuffmanBlock(pBlockDesc desc)
	: flattenBlock(desc)
{
}
seHuffmanBlock::~seHuffmanBlock()
{
}
}		// core
}		// msdb