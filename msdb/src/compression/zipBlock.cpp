#include <pch.h>
#include <compression/zipBlock.h>

namespace msdb
{
namespace core
{
zipBlock::zipBlock(pBlockDesc desc)
	: memBlock(desc)
{
}

zipBlock::~zipBlock()
{
}
}		// core
}		// msdb