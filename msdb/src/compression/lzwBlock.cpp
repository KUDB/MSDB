#include <pch.h>
#include <compression/lzwBlock.h>
#include <util/ioutil.h>

namespace msdb
{
namespace core
{
lzwBlock::lzwBlock(pBlockDesc desc)
	: memBlock(desc)
{
}
lzwBlock::~lzwBlock()
{
}
}		// core
}		// msdb