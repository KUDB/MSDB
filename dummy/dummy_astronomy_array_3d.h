#pragma once
#ifndef _MSDB_DUMMY_ASTRONOMY_3D_H_
#define _MSDB_DUMMY_ASTRONOMY_3D_H_

#include <api_cpp/msdb.h>
#include <dummy_array.h>

#define DUMMY_PATH(path) "../../dummy/"#path

namespace msdb
{
namespace dummy
{
namespace data_star64x64x64
{
static const std::string arrName = "star64x64x64";
static const core::arrayId aid = 30000;
static const std::string filePath = DUMMY_PATH(Binary_Star_STSCI_H_p1942a_f_1024x1024.data);

static const core::dimension dims({ 64, 64, 64});
static const core::dimension chunkDims({ 32, 32, 32 });
static const core::dimension blockDims({ 32, 32, 32 });
static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Z";
static const std::string nameDim_1 = "Y";
static const std::string nameDim_2 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

// TODO::Remove getParam
std::pair<int, int> getParam(encodingType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getInsertSaveAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	encodingType compType, attrIndexType idxType);
}		// data_star64x64x64

namespace data_nexrad_16x1024x2048
{
static const std::string arrName = "nexrad_16x1024x2048";
static const core::arrayId aid = 31000;
//static const std::string filePath = DUMMY_PATH(Binary_nexrad_3d_v3_1_20151101T060000Z.data);
static const std::string filePath = DUMMY_PATH(Binary_nexrad_3d_v3_1_20171201T000000Z.data);

static const core::dimension dims({ 24, 1024, 2048 });
static const core::dimension chunkDims({ 8, 32, 32 });
static const core::dimension blockDims({ 8, 32, 32 });
//static const core::dimension dims({ 16, 1024, 2048 });
//static const core::dimension chunkDims({ 16, 32, 32 });
//static const core::dimension blockDims({ 16, 32, 32 });
static const int wtLevel = 2;
static const int mmtLevel = 2;
static const int compassBins = 128;

static const std::string nameDim_0 = "Z";
static const std::string nameDim_1 = "Y";
static const std::string nameDim_2 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

// TODO::Remove getParam
std::pair<int, int> getParam(encodingType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getInsertSaveAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	encodingType compType, attrIndexType idxType);
}		// data_nexrad_16x1024x2048

namespace data_solar_sdo_1024x1024x512
{
static const std::string arrName = "solar_sdo_1024x1024x512";
static const core::arrayId aid = 32000;
static const std::string filePath = DUMMY_PATH(Binary_SOLAR_SDO_AIA171_2200101_05_latest_1024x1024x512.data);

static const core::dimension dims({ 512, 1024, 1024 });
static const core::dimension chunkDims({ 32, 64, 64 });
static const core::dimension blockDims({ 32, 64, 64 });
static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Z";
static const std::string nameDim_1 = "Y";
static const std::string nameDim_2 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

// TODO::Remove getParam
std::pair<int, int> getParam(encodingType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getInsertSaveAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	encodingType compType, attrIndexType idxType);
}		// data_solar_sdo_1024x1024x512

namespace data_nexrad_201711_64x24x1024x2048
{
static const std::string arrName = "nexrad_201711_64x24x1024x2048";
static const core::arrayId aid = 33000;
//static const std::string filePath = DUMMY_PATH(Binary_nexrad_3d_v3_1_20151101T060000Z.data);
static const std::string filePath = DUMMY_PATH(Binary_nexrad_3d_v3_1_201711.data);

static const core::dimension dims({ 64, 24, 1024, 2048 });
static const core::dimension chunkDims({ 8, 8, 64, 64 });
static const core::dimension blockDims({ 8, 8, 64, 64 });
static const int wtLevel = 2;
static const int mmtLevel = 2;
static const int compassBins = 128;

static const std::string nameDim_0 = "T";
static const std::string nameDim_1 = "Z";
static const std::string nameDim_2 = "Y";
static const std::string nameDim_3 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({4, 32});

// TODO::Remove getParam
std::pair<int, int> getParam(encodingType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getInsertSaveAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	encodingType compType = encodingType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	encodingType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	encodingType compType, attrIndexType idxType);
}		// data_nexrad_201711_64x24x1024x2048
}		// dummy
}		// msdb

#endif	// _MSDB_DUMMY_ASTRONOMY_3D_H_