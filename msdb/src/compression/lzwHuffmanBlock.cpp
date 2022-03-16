#include <pch.h>
#include <compression/lzwHuffmanBlock.h>

namespace msdb
{
namespace core
{
lzwHuffmanBlock::lzwHuffmanBlock(pBlockDesc desc)
	: flattenBlock(desc)
{
}
lzwHuffmanBlock::~lzwHuffmanBlock()
{
}
}		// core
}		// msdb