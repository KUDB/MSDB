#include <pch.h>
#include <compression/zipBlock.h>

namespace msdb
{
namespace core
{
zipBlock::zipBlock(pBlockDesc desc)
	: flattenBlock(desc)
{
}

zipBlock::~zipBlock()
{
}
}		// core
}		// msdb