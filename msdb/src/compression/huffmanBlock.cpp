#include <pch.h>
#include <compression/huffmanBlock.h>

namespace msdb
{
namespace core
{
huffmanBlock::huffmanBlock(pBlockDesc desc)
	: memBlock(desc)
{
}

huffmanBlock::~huffmanBlock()
{

}
}		// core
}		// msdb