#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
#define _MSDB_OP_WAVELET_ENCODE_ARRAY_H_

#include <pch.h>
#include <array/memArray.h>

namespace msdb
{
namespace core
{
class wavelet_encode_array : public memArray
{
public:
	using base_type = memArray;

public:
	wavelet_encode_array(pArrayDesc desc);

	/*
	 * In wavelet_encode_array,
	 * number of chunks are multipled by (2)^(level)^(num of dims).
	 *
	 * For example, there is 2-dimensional array.
	 *
	 * [Level 0]
	 * In level 0, number of chunks is sustained.
	 * ┏━━━┓	┏━┳━┓	┏━━━┓
	 * ┃ 0 ┃ →	┣━╋━┫ →	┃ 0 ┃
	 * ┗━━━┛	┗━┻━┛	┗━━━┛
	 *
	 * [Level 1]
	 * In level 1, number of chunks increased into 4.
	 * ┏━━━━━━━┓	┏━┳━┳━┯━┓	┏━━━┳━━━┓
	 * ┃       ┃	┣━╋━╉─┼─┨	┃ 0 ┃ 1 ┃
	 * ┃   0   ┃ →	┣━╇━╋━┿━┫ →	┣━━━╋━━━┫
	 * ┃       ┃	┠─┼─╂─┼─┨	┃ 2 ┃ 3 ┃
	 * ┗━━━━━━━┛	┗━┷━┻━┷━┛	┗━━━┻━━━┛
	 *
	 * Example 01>
	 * weChunks for (0,0) chunk
	 * ┏━━━┳━━━┓	┏━┳━┳━┑
	 * ┃ * ┃   ┃	┣━╋━╉─┘
	 * ┣━━━╋━━━┫ →	┣━╇━╋━┑
	 * ┃   ┃   ┃	┖─┘ ┖─┘
	 * ┗━━━┻━━━┛
	 *
	 * Example 02>
	 * weChunks for (0,1) chunk
	 * ┏━━━┳━━━┓	┏━┳━┓ ┍━┓
	 * ┃   ┃ * ┃	┣━╋━┫ └─┚
	 * ┣━━━╋━━━┫ →	┗━╇━┫ ┍━┓
	 * ┃   ┃   ┃	  └─┚ └─┚
	 * ┗━━━┻━━━┛
	 */

public:
	virtual pChunk makeChunk(const attributeId attrId, const chunkId cId) override;
	virtual pChunk makeChunk(const chunkDesc& desc) override;

public:
	size_t getMaxLevel();
	dimension getOrigianlChunkDims();
	void setLevel(const size_t maxLevel);
	void setOrigianlChunkDims(const dimension originalChunkDims);

private:
	bool isMaxLevelAvailable(size_t maxLevel);

protected:
	size_t maxLevel_;
	dimension originalChunkDims_;
};
}		// core
}		// msdb
#endif		// _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
