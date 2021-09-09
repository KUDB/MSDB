#pragma once
#ifndef _MSDB_DIMENSIONDESC_H_
#define _MSDB_DIMENSIONDESC_H_

#include <pch.h>
#include <util/coordinate.h>
#include <array/dimensionId.h>
#include <array/dimension.h>
#include <xml/tinyxml2.h>

namespace msdb
{
namespace core
{
#define _MSDB_STR_DIMENSION_DESCS_	"DIMENSION_DESCRIPTIONS"
#define _MSDB_STR_DIM_DESC_			"DIMENSION_DESCRIPTION"
#define _MSDB_STR_DIM_ID_			"DIMENSION_ID"
#define _MSDB_STR_DIM_NAME_			"DIMENSION_NAME"
#define _MSDB_STR_DIM_START_		"DIMENSION_START"
#define _MSDB_STR_DIM_END_			"DIMENSION_END"
#define _MSDB_STR_DIM_CHUNK_SIZE_	"DIMENSION_CHUNK_SIZE"
#define _MSDB_STR_DIM_BLOCK_SIZE_	"DIMENSION_BLOCK_SIZE"

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
	std::string toString();

	/**
	 * Save/load in XML file
	 */
	tinyxml2::XMLElement* convertToXMLDoc(tinyxml2::XMLElement* node);
	static pDimensionDesc buildDescFromXML(tinyxml2::XMLElement* node);

	bool operator == (const dimensionDesc& right_);

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