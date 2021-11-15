#pragma once

#ifndef _MSDB_DUMMY_ARRAY_H_
#define _MSDB_DUMMY_ARRAY_H_

#include <vector>
#include <string>
#include <utility>
#include <array/arrayDesc.h>
#include <api_cpp/msdb.h>

#define DUMMY_PATH(path) "../dummy/"#path

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

core::pDimensionDescs dimensionDescBuilder(
	const std::vector<std::string> dimNames, const core::dimension& dims,
	const core::dimension& chunkDims, const core::dimension& blockDims
);
core::pAttributeDescs attributeDescBuilder(
	const std::vector<std::string> attrNames,
	const std::vector<core::eleType> attrTypes
);

std::pair<core::arrayId, std::string> getArrayIdName(core::compressionType compType, core::arrayId baseId, std::string baseName);

Array getArrayAFL(
	std::string arrName, compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertAFL(
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

namespace data_star1024x1024
{
static const std::string arrName = "star1024x1024";
static const core::arrayId aid = 1000;
static const std::string filePath = DUMMY_PATH(Binary_Star_STSCI_H_p1942a_f_1024x1024.data);

static const core::dimension dims({ 1024, 1024 });
static const core::dimension chunkDims({ 128, 128 });
static const core::dimension blockDims({ 128, 128 });
static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::eleType typeAttr_0 = core::eleType::UINT8;

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}

namespace data_solar1024x1024
{
static const std::string arrName = "solar1024x1024";
static const core::arrayId aid = 2000;
static const std::string filePath = DUMMY_PATH(Binary_Solar_latest10240171_1024x1024.data);

static const core::dimension dims(1024, 1024);
static const core::dimension chunkDims(128, 128);
static const core::dimension blockDims(128, 128);

static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::eleType typeAttr_0 = core::eleType::UINT8;

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}		// data_solar1024x1024

namespace data_saturn1024x1024
{
static const std::string arrName = "saturn1024x1024";
static const core::arrayId aid = 3000;
static const std::string filePath = DUMMY_PATH(Binary_Saturn_STSCI_H_p1936a_f_1024x1024.data);

static const core::dimension dims(1024, 1024);
static const core::dimension chunkDims(128, 128);
static const core::dimension blockDims(128, 128);

static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::eleType typeAttr_0 = core::eleType::UINT8;

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}		// data_saturn1024x1024

namespace data_mars4096x2048
{
static const std::string arrName = "mars4096x2048";
static const core::arrayId aid = 4000;
static const std::string filePath = DUMMY_PATH(Binary_Mars_Viking_MDIM21_ClrMosaic_1km_4096x2048.data);

static const core::dimension dims(2048, 4096);
static const core::dimension chunkDims(128, 128);
static const core::dimension blockDims(128, 128);

static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::eleType typeAttr_0 = core::eleType::UINT8;

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}		// data_mars4096x2048

namespace data_mercury20480x10240
{
static const std::string arrName = "mercury20480x10240";
static const core::arrayId aid = 5000;
static const std::string filePath = DUMMY_PATH(Binary_Mercury_Messenger_DEM_Global_665m_v2_max_23040x11520.data);

static const core::dimension dims(10240, 20480);
static const core::dimension chunkDims(256, 256);
static const core::dimension blockDims(256, 256);

static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::eleType typeAttr_0 = core::eleType::UINT8;

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}		// data_mercury20480x10240

namespace data_lunar102400x40960
{
static const std::string arrName = "lunar102400x40960";
static const core::arrayId aid = 6000;
static const std::string filePath = DUMMY_PATH(Binary_Lunar_LRO_WAC_GLD100_DTM_79S79N_100m_v1_109165x47912.data);

static const core::dimension dims(32768, 98304);
static const core::dimension chunkDims(512, 512);
static const core::dimension blockDims(512, 512);

static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::eleType typeAttr_0 = core::eleType::UINT16;

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}		// data_lunar102400x40960
}		// dummy
}		// msdb
#endif	// _MSDB_DUMMY_ARRAY_H_