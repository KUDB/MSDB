#include <pch.h>
#include <compression/lzwHuffmanBlock.h>

namespace msdb
{
namespace core
{
lzwHuffmanBlock::lzwHuffmanBlock(pBlockDesc desc)
	: memBlock(desc)
{
}
lzwHuffmanBlock::~lzwHuffmanBlock()
{
}
}		// core
}		// msdb