#include <pch.h>
#include <array/chunkType.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// chunkType
//////////////////////////////
chunkType::chunkType(pChunkFactory myFactory)
	: myFactory_(myFactory)
{
}
chunkType::~chunkType()
{
	this->myFactory_ = nullptr;
}
}		// core
}		// msdb