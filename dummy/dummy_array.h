#pragma once

#ifndef _MSDB_DUMMY_ARRAY_H_
#define _MSDB_DUMMY_ARRAY_H_

#include <vector>
#include <map>
#include <dummy_array_util.h>

#define DUMMY_PATH(path) "../dummy/"#path

namespace msdb
{
namespace dummy
{
class dummy_array
{
public:
	Array getArrayAFL(
		std::string arrName, encodingType compType = encodingType::RAW);
	std::shared_ptr<AFLOperator> getInsertSaveAFL(std::string arrName, std::string filePath,
		encodingType compType = encodingType::RAW,
		int paramOne = 0, int paramTwo = 0);
	std::shared_ptr<AFLOperator> getLoadAFL(std::string arrName,
		encodingType compType = encodingType::RAW,
		int paramOne = 0, int paramTwo = 0);
	std::shared_ptr<AFLOperator> getBuildIndexAFL(std::string arrName, std::string filePath,
		encodingType compType, attrIndexType idxType,
		int paramOne = 0, int paramTwo = 0);
	std::shared_ptr<AFLOperator> getSaveIndexAFL(std::string arrName,
		encodingType compType, attrIndexType idxType,
		int paramOne = 0, int paramTwo = 0);
	std::shared_ptr<AFLOperator> getLoadIndexAFL(std::string arrName,
		encodingType compType, attrIndexType idxType,
		int paramOne = 0, int paramTwo = 0);

	// Used in Array Build
	std::vector<DefDimension> getDefDimensions();
	std::vector<DefAttribute> getDefAttributes();

	// TODO::saveRawFile
	//bool saveRawFile(std::string filePath);

	//////////////////////////////
	// Virtual Functions
	virtual core::attributeDesc::paramType getAttrParam(core::attributeId attrId) = 0;
	virtual std::shared_ptr<AFLOperator> getArrayBuildAFL(
		std::vector<materializedType> matTypes, std::vector<encodingType> compType) = 0;
	virtual core::pArrayDesc getDummyArrayDesc();
	
public:
	dummy_array(const std::string arrName, const core::arrayId aid,
		const core::dimension& dims, const core::dimension& chunkDims, const core::dimension& blockDims,
		const std::vector<std::string>& nameDim, const std::vector<std::string>& nameAttr,
		const std::vector<core::dataType>& typeAttr,
		const std::vector<core::materializedType>& matTypes = {},
		const std::vector<core::encodingType>& compTypes = {},
		const std::vector<core::attributeDesc::paramType>& attrParams = {});

public:
	const std::string _arrName;
	const core::arrayId _aid;

	const core::dimension _dims;
	const core::dimension _chunkDims;
	const core::dimension _blockDims;

	std::vector<std::string> _nameDim;
	std::vector<std::string> _nameAttr;
	std::vector<core::dataType> _typeAttr;
	std::vector<core::materializedType> _matTypes;
	std::vector<core::encodingType> _compTypes;

	std::vector<core::attributeDesc::paramType> _attrParams;
};
}		// dummy
}		// msdb
#endif	// _MSDB_DUMMY_ARRAY_H_