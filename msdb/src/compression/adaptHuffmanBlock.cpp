#include <pch.h>
#include <compression/adaptHuffmanBlock.h>

namespace msdb
{
namespace core
{
adaptHuffmanBlock::adaptHuffmanBlock(pBlockDesc desc)
	: flattenBlock(desc)
{
}

adaptHuffmanBlock::~adaptHuffmanBlock()
{

}
}		// core
}		// msdb