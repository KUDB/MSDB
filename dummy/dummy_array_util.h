#pragma once

#ifndef _MSDB_DUMMY_ARRAY_UTIL_H_
#define _MSDB_DUMMY_ARRAY_UTIL_H_

#include <vector>
#include <string>
#include <utility>
#include <array/arrayDesc.h>
#include <api_cpp/msdb.h>

namespace msdb
{
namespace dummy
{
static const int arr_id_seacow = 10;
static const int arr_id_seacow_huffman = 11;
static const int arr_id_spiht = 20;
static const int arr_id_compass = 25;
static const int arr_id_lzw = 30;
static const int arr_id_huffman = 35;
static const int arr_id_adapt_huffman = 36;
static const int arr_id_lzw_huffman = 40;
static const int arr_id_zip = 45;

Array getArrayAFL(
	std::string arrName, compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertSaveAFL(
	std::string arrName, std::string filePath,
	compressionType compType = compressionType::RAW,
	int paramOne = 0, int paramTwo = 0);
std::shared_ptr<AFLOperator> getLoadAFL(
	std::string arrName,
	compressionType compType = compressionType::RAW,
	int paramOne = 0, int paramTwo = 0);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	std::string arrName, std::string filePath,
	compressionType compType, attrIndexType idxType,
	int paramOne = 0, int paramTwo = 0);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	std::string arrName,
	compressionType compType, attrIndexType idxType,
	int paramOne = 0, int paramTwo = 0);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	std::string arrName,
	compressionType compType, attrIndexType idxType,
	int paramOne = 0, int paramTwo = 0);

core::pDimensionDescs dimensionDescBuilder(
	const std::vector<std::string> dimNames, const core::dimension& dims,
	const core::dimension& chunkDims, const core::dimension& blockDims
);
core::pAttributeDescs attributeDescBuilder(
	const std::vector<std::string>& attrNames,
	const std::vector<core::dataType>& attrTypes);
core::pAttributeDescs attributeDescBuilder(const std::vector<std::string>& attrNames,
										   const std::vector<core::dataType>& attrTypes,
										   const std::vector<core::materializedType>& matTypes,
										   const std::vector<core::compressionType>& compTypes,
										   const std::vector<core::attributeDesc::paramType>& optionalParams = {});
std::pair<core::arrayId, std::string> getArrayIdName(
	core::compressionType compType, 
	core::arrayId baseId, 
	std::string baseName
);
}		// dummy
}		// msdb
#endif	// _MSDB_DUMMY_ARRAY_UTIL_H_