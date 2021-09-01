#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_ARRAY_H_
#define _MSDB_OP_WAVELET_DECODE_ARRAY_H_

#include <pch.h>
#include <array/memBlockArray.h>

namespace msdb
{
namespace core
{
class wavelet_decode_array : public memBlockArray
{
public:
	using base_type = memBlockArray;

public:
	wavelet_decode_array(pArrayDesc desc, const size_t maxLevel);

public:
	size_t getMaxLevel();
	//chunkId getChunkId(pChunkDesc cDesc, size_t level, size_t band);
	//chunkId getChunkIdFromItemCoor(coor& itemCoor, size_t level, size_t band);
	//chunkId getChunkIdFromChunkCoor(coor& chunkCoor, size_t level, size_t band);

protected:
	size_t maxLevel_;
	//pArray weArray;		// wavelet encoded array (source array)
						// hold source array to decode only some part of source array
};
}		// core
}		// msdb
#endif		// _MSDB_OP_WAVELET_DECODE_ARRAY_H_