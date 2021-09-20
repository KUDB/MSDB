#include <pch.h>
#include <compression/seHuffmanBlock.h>

namespace msdb
{
namespace core
{
seHuffmanBlock::seHuffmanBlock(pBlockDesc desc)
	: memBlock(desc)
{
}
seHuffmanBlock::~seHuffmanBlock()
{
}
}		// core
}		// msdb