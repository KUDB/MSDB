﻿#include <pch.h>
#include <op/wavelet_encode/wtChunk.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// wtChunkType
//
wtChunkType::wtChunkType(const dataType& type)
	: chunkType(buildFactory(wtChunkFactoryBuilder(), type))
{
}

//REGISTER_CHUNK_FACTORY("wtChunk", wtChunkFactoryBuilder())

//wtChunk::wtChunk(pChunkDesc desc)
//	: monoChunk(desc), level_(0)
//{
//}
//
//wtChunk::~wtChunk()
//{
//}
//
//size_t wtChunk::getLevel()
//{
//	return this->level_;
//}
////chunkId wtChunk::getBandId()
////{
////	return this->bandId_;
////}
////chunkId wtChunk::getSourceChunkId()
////{
////	return this->sourceChunkId_;
////}
//
//void wtChunk::setLevel(size_t level)
//{
//	this->level_ = level;
//}
////void wtChunk::setBandId(chunkId bid)
////{
////	this->bandId_ = bid;
////}
////void wtChunk::setSourceChunkId(chunkId cid)
////{
////	this->sourceChunkId_ = cid;
////}
//dimension wtChunk::getTileSpace(dimension sourceChunkDim)
//{
//	dimension tileSpace(sourceChunkDim);
//	dimension tileDim(sourceChunkDim);
//	tileDim /= pow(2, this->level_ + 1);
//	tileSpace /= tileDim;
//
//	return tileSpace;
//}
}		// core
}		// msdb