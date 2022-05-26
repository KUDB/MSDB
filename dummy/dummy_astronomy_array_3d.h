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
static const core::arrayId aid = 3000;
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

// TODO::Remove getParam
std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertSaveAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}		// data_star64x64x64

namespace data_nexrad_16x1024x2048
{
static const std::string arrName = "nexrad_16x1024x2048";
static const core::arrayId aid = 3100;
static const std::string filePath = DUMMY_PATH(Binary_nexrad_3d_v3_1_20171201T000000Z.data);

static const core::dimension dims({ 16, 1024, 2048 });
static const core::dimension chunkDims({ 16, 32, 32 });
static const core::dimension blockDims({ 16, 32, 32 });
static const int wtLevel = 3;
static const int mmtLevel = 3;
static const int compassBins = 128;

static const std::string nameDim_0 = "Z";
static const std::string nameDim_1 = "Y";
static const std::string nameDim_2 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::dataType typeAttr_0 = core::concreteTy<uint8_t>();

// TODO::Remove getParam
std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	materializedType matType = materializedType::FLATTEN,
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getInsertSaveAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getLoadAFL(
	compressionType compType = compressionType::RAW);
std::shared_ptr<AFLOperator> getBuildIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getSaveIndexAFL(
	compressionType compType, attrIndexType idxType);
std::shared_ptr<AFLOperator> getLoadIndexAFL(
	compressionType compType, attrIndexType idxType);
}		// data_nexrad_16x1024x2048
}		// dummy
}		// msdb

#endif	// _MSDB_DUMMY_ASTRONOMY_3D_H_