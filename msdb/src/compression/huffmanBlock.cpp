#include <pch.h>
#include <compression/huffmanBlock.h>

namespace msdb
{
namespace core
{
huffmanBlock::huffmanBlock(pBlockDesc desc)
	: flattenBlock(desc)
{
}

huffmanBlock::~huffmanBlock()
{

}
}		// core
}		// msdb