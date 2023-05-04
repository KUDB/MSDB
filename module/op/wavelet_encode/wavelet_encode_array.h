#pragma once
#ifndef _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
#define _MSDB_OP_WAVELET_ENCODE_ARRAY_H_

#include <pch_op.h>
#include <array/flattenArray.h>

namespace msdb
{
namespace core
{
class OP_API wavelet_encode_array : public flattenArray
{
public:
	using base_type = flattenArray;

public:
	wavelet_encode_array(pArrayDesc desc);
	virtual ~wavelet_encode_array();

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
	dimension getOrigianlChunkDims();
	void setOrigianlChunkDims(const dimension originalChunkDims);

public:
	virtual void initChunkFactories();

protected:
	dimension originalChunkDims_;
};
}		// core
}		// msdb
#endif	// _MSDB_OP_WAVELET_ENCODE_ARRAY_H_
