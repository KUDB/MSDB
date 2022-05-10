#pragma once
#ifndef _MSDB_DUMMY_ASTRONOMY_H_
#define _MSDB_DUMMY_ASTRONOMY_H_

#include <api_cpp/msdb.h>
#include <dummy_array.h>

#define DUMMY_PATH(path) "../dummy/"#path

namespace msdb
{
namespace dummy
{
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
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
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
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
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
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
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
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
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

static const core::dimension dims(8192, 16384);
static const core::dimension chunkDims(256, 256);
static const core::dimension blockDims(256, 256);

static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
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
static const int compassBins = 256;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint16_t>();

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
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
#endif	// _MSDB_DUMMY_STAR_H_