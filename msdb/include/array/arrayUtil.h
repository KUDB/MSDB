#pragma once
#ifndef _MSDB_ARRAYUTIL_H_
#define _MSDB_ARRAYUTIL_H_

#include <pch.h>
#include <array/arrayDesc.h>
#include <array/chunkIterator.h>
#include <util/coordinate.h>

namespace msdb
{
namespace core
{
template <typename Dty_>
size_t calcNumItems(const Dty_* dims, const size_t dSize)
{
	size_t length = 1;
	for (size_t d = 0; d < dSize; d++)
	{
		length *= dims[d];
	}
	return length;
}

template <typename Dty_>
std::vector<Dty_> calcChunkNums(const Dty_* dims, const Dty_* chunkDims, const size_t dSize)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / chunkDims[d]);	// ceiling
	}

	return output;
}

template <typename Dty_>
std::vector<Dty_> calcChunkNums(const Dty_* dims, const Dty_* leafChunkDims, const size_t dSize, const size_t level)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / leafChunkDims[d] / pow(2, level));	// ceiling
	}

	return output;
}

template <typename Dty_>
std::vector<Dty_> calcChunkDims(const Dty_* dims, const Dty_* chunkNums, const size_t dSize)
{
	std::vector<Dty_> output;

	for (size_t d = 0; d < dSize; d++)
	{
		output.push_back(1 + (dims[d] - 1) / chunkNums[d]);	// ceiling
	}

	return output;
}
}		// core
}		// msdb
#endif	// _MSDB_ARRAYUTIL_H_