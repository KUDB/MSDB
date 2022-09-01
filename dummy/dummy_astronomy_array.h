#pragma once
#ifndef _MSDB_DUMMY_ASTRONOMY_H_
#define _MSDB_DUMMY_ASTRONOMY_H_

#include <api_cpp/msdb.h>
#include <dummy_array.h>

#define DUMMY_PATH(path) "../../dummy/"#path

namespace msdb
{
namespace dummy
{
namespace data_star1024x1024
{
static const std::string arrName = "star1024x1024";
static const core::arrayId aid = 21000;
static const std::string filePath = DUMMY_PATH(Binary_Star_STSCI_H_p1942a_f_1024x1024.data);

extern const core::dimension dims;
extern const core::dimension chunkDims;
extern const core::dimension blockDims;

extern const int wtLevel;
extern const int mmtLevel;
extern const int compassBins;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
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
}

namespace data_saturn1024x1024
{
static const std::string arrName = "saturn1024x1024";
static const core::arrayId aid = 22000;
static const std::string filePath = DUMMY_PATH(Binary_Saturn_STSCI_H_p1936a_f_1024x1024.data);

extern const core::dimension dims;
extern const core::dimension chunkDims;
extern const core::dimension blockDims;

extern const int wtLevel;
extern const int mmtLevel;
extern const int compassBins;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

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
}		// data_saturn1024x1024

namespace data_solar1024x1024
{
static const std::string arrName = "solar4096x4096";
static const core::arrayId aid = 23000;
static const std::string filePath = DUMMY_PATH(Binary_SOLAR_220530_002315_4096_HMID.data);

extern const core::dimension dims;
extern const core::dimension chunkDims;
extern const core::dimension blockDims;

extern const int wtLevel;
extern const int mmtLevel;
extern const int compassBins;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

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
}		// data_solar1024x1024

namespace data_mars4096x2048
{
static const std::string arrName = "mars4096x2048";
static const core::arrayId aid = 24000;
static const std::string filePath = DUMMY_PATH(Binary_Mars_Viking_MDIM21_ClrMosaic_1km_4096x2048.data);

extern const core::dimension dims;
extern const core::dimension chunkDims;
extern const core::dimension blockDims;

extern const int wtLevel;
extern const int mmtLevel;
extern const int compassBins;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

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
}		// data_mars4096x2048

namespace data_mercury20480x10240
{
static const std::string arrName = "mercury20480x10240";
static const core::arrayId aid = 25000;
static const std::string filePath = DUMMY_PATH(Binary_Mercury_Messenger_DEM_Global_665m_v2_max_23040x11520.data);

extern const core::dimension dims;
extern const core::dimension chunkDims;
extern const core::dimension blockDims;

extern const int wtLevel;
extern const int mmtLevel;
extern const int compassBins;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

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
}		// data_mercury20480x10240

namespace data_lunar102400x40960
{
static const std::string arrName = "lunar102400x40960";
static const core::arrayId aid = 26000;
static const std::string filePath = DUMMY_PATH(Binary_Lunar_LRO_WAC_GLD100_DTM_79S79N_100m_v1_109165x47912.data);

extern const core::dimension dims;
extern const core::dimension chunkDims;
extern const core::dimension blockDims;

extern const int wtLevel;
extern const int mmtLevel;
extern const int compassBins;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint16_t>();
static const std::vector<uint8_t> valueRange({ std::numeric_limits<uint8_t>::max() / 2, std::numeric_limits<uint8_t>::max() });

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
}		// data_lunar102400x40960
}		// dummy
}		// msdb
#endif	// _MSDB_DUMMY_STAR_H_