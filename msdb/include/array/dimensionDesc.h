#pragma once
#ifndef _MSDB_DIMENSIONDESC_H_
#define _MSDB_DIMENSIONDESC_H_

#include <pch.h>
#include <util/coordinate.h>
#include <array/dimensionId.h>
#include <array/dimension.h>

namespace msdb
{
namespace core
{
class dimensionDesc;
using pDimensionDesc = std::shared_ptr<dimensionDesc>;

class dimensionDescs;
using pDimensionDescs = std::shared_ptr<dimensionDescs>;

class dimensionDescs : public std::vector<pDimensionDesc>, public std::enable_shared_from_this<dimensionDescs>
{
public:
	dimensionDescs();
	dimensionDescs(const dimensionDescs& mit);

public:
	dimension getDims();
	dimension getChunkDims();
	dimension getChunkSpace();
	dimension getBlockDims();
	dimension getBlockSpace();
};

class dimensionDesc
{
public:
	using dimension_type = position_t;

public:
	dimensionDesc(dimensionId id, std::string name, 
				  dimension_type start, dimension_type end,
				  position_t chunkSize, position_t blockSize);

	dimensionDesc(const dimensionDesc& mit);

public:
	position_t getLength();
	size_t getChunkNum();

public:
	dimensionId id_;
	std::string name_;
	dimension_type start_;
	dimension_type end_;
	position_t chunkSize_;
	position_t blockSize_;
};
}		// core
}		// msdb
#endif	// _MSDB_DIMENSIONDESC_H_