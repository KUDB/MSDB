#include <pch.h>
#include <compression/wtChunk.h>

namespace msdb
{
namespace core
{
wtChunk::wtChunk(pChunkDesc desc)
	: memChunk(desc), level_(0)
{
}

wtChunk::~wtChunk()
{
}

size_t wtChunk::getLevel()
{
	return this->level_;
}
//chunkId wtChunk::getBandId()
//{
//	return this->bandId_;
//}
//chunkId wtChunk::getSourceChunkId()
//{
//	return this->sourceChunkId_;
//}

void wtChunk::setLevel(size_t level)
{
	this->level_ = level;
}
//void wtChunk::setBandId(chunkId bid)
//{
//	this->bandId_ = bid;
//}
//void wtChunk::setSourceChunkId(chunkId cid)
//{
//	this->sourceChunkId_ = cid;
//}
dimension wtChunk::getTileSpace(dimension sourceChunkDim)
{
	dimension tileSpace(sourceChunkDim);
	dimension tileDim(sourceChunkDim);
	tileDim /= pow(2, this->level_ + 1);
	tileSpace /= tileDim;

	return tileSpace;
}
}		// core
}		// msdb