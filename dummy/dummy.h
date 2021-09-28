#pragma once
#include <vector>
#include <string>
#include <array/arrayDesc.h>

#ifndef _MSDB_EXAMPLE_DUMMY_H_
#define _MSDB_EXAMPLE_DUMMY_H_

#define DUMMY_PATH(path) "../dummy/"#path


namespace msdb
{
namespace dummy
{
core::pDimensionDescs dimensionDescBuilder(
	std::vector<std::string> dimNames, core::dimension& dims,
	core::dimension& chunkDims, core::dimension& blockDims
);
core::pAttributeDescs attributeDescBuilder(
	std::vector<std::string> attrNames,
	std::vector<core::eleType> attrTypes
);

namespace data_star1024x1024
{
static const std::string arrName = "star1024x1024";
static const core::arrayId aid = 1000;
static const std::string filePath = DUMMY_PATH(Binary_Star_STSCI_H_p1942a_f_1024x1024.data);

static const core::dimension dims(1024, 1024);
static const core::dimension chunkDims(128, 128);
static const core::dimension blockDims(32, 32);
static const int wtLevel = 3;
static const int mmtLevel = 3;

core::pArrayDesc getDummyArrayDesc();
}		// data_star1024x1024

namespace data_solar1024x1024
{
static const std::string arrName = "solar1024x1024";
static const core::arrayId aid = 2000;
static const std::string filePath = DUMMY_PATH(Binary_Solar_latest10240171_1024x1024.data);

static const core::dimension dims(1024, 1024);
static const core::dimension chunkDims(128, 128);
static const core::dimension blockDims(32, 32);

core::pArrayDesc getDummyArrayDesc();
}		// data_solar1024x1024

namespace data_saturn1024x1024
{
static const std::string arrName = "saturn1024x1024";
static const core::arrayId aid = 3000;
static const std::string filePath = DUMMY_PATH(Binary_Saturn_STSCI_H_p1936a_f_1024x1024.data);

static const core::dimension dims(1024, 1024);
static const core::dimension chunkDims(128, 128);
static const core::dimension blockDims(32, 32);

core::pArrayDesc getDummyArrayDesc();
}		// data_saturn1024x1024

namespace data_mars4096x2048
{
static const std::string arrName = "mars4096x2048";
static const core::arrayId aid = 4000;
static const std::string filePath = DUMMY_PATH(Binary_Mars_Viking_MDIM21_ClrMosaic_1km_4096x2048.data);

static const core::dimension dims(2048, 4096);
static const core::dimension chunkDims(128, 128);
static const core::dimension blockDims(32, 32);

core::pArrayDesc getDummyArrayDesc();
}		// data_mars4096x2048

namespace data_mercury20480x10240
{
static const std::string arrName = "mercury20480x10240";
static const core::arrayId aid = 5000;
static const std::string filePath = DUMMY_PATH(Binary_Mercury_Messenger_DEM_Global_665m_v2_max_23040x11520.data);

static const core::dimension dims(10240, 20480);
static const core::dimension chunkDims(256, 256);
static const core::dimension blockDims(64, 64);

core::pArrayDesc getDummyArrayDesc();
}		// data_mercury20480x10240

namespace data_lunar102400x40960
{
static const std::string arrName = "lunar102400x40960";
static const core::arrayId aid = 6000;
static const std::string filePath = DUMMY_PATH(Binary_Lunar_LRO_WAC_GLD100_DTM_79S79N_100m_v1_109165x47912.data);

static const core::dimension dims(40960, 102400);
static const core::dimension chunkDims(512, 512);
static const core::dimension blockDims(128, 128);

core::pArrayDesc getDummyArrayDesc();
}		// data_lunar102400x40960
}		// dummy
}		// msdb

#endif	// _MSDB_EXAMPLE_DUMMY_H_