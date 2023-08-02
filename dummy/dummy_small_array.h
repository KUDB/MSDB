#pragma once

#ifndef _MSDB_DUMMY_SMALL_ARRAY_H_
#define _MSDB_DUMMY_SMALL_ARRAY_H_

#include <iostream>
#include <string>
#include "dummy_array.h"

namespace msdb
{
namespace dummy
{
namespace data_8x8
{
const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 128;

const int numDims = 2;		// 2-dimensional
}

class array_8x8 : public dummy_array
{
public:
	virtual core::attributeDesc::paramType getAttrParam(core::attributeId attrId);

	virtual std::shared_ptr<AFLOperator> getArrayBuildAFL(
		std::vector<materializedType> matTypes = {}, std::vector<encodingType> compType = {});

public:
	array_8x8(const std::string arrName, 
		const core::arrayId aid,
		const core::dimension& chunkDims, 
		const core::dimension& blockDims,
		const std::vector<std::string>& nameDim, 
		const std::vector<std::string>& nameAttr,
		const std::vector<core::dataType>& typeAttr,
		const std::vector<core::materializedType>& matTypes,
		const std::vector<core::encodingType>& compTypes,
		const std::vector<core::attributeDesc::paramType>& attrParams
	);
};
//////////////////
namespace data_1x32x32
{
const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 128;

const int numDims = 3;		// 2-dimensional
}
class array_1x32x32 : public dummy_array
{
public:
	virtual core::attributeDesc::paramType getAttrParam(core::attributeId attrId);

	virtual std::shared_ptr<AFLOperator> getArrayBuildAFL(
		std::vector<materializedType> matTypes = {}, std::vector<encodingType> compType = {});

public:
	array_1x32x32(const std::string arrName,
		const core::arrayId aid,
		const std::vector<core::attributeDesc::paramType>& attrParams
	);
};
}		// dummy
}		// msdb
#endif	// _MSDB_DUMMY_SMALL_ARRAY_H_