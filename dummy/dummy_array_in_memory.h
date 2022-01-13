#pragma once

#ifndef _MSDB_DUMMY_ARRAY_IN_MEMORY_H_
#define _MSDB_DUMMY_ARRAY_IN_MEMORY_H_

#include <dummy_array_util.h>

namespace msdb
{
namespace dummy
{
namespace array_mem_char_4x4
{
static const std::string arrName = "mem_char_1024x1024";
static const core::arrayId aid = 10000;

static const core::dimension dims({ 4, 4 });
static const core::dimension chunkDims({ 4, 4 });
static const core::dimension blockDims({ 4, 4 });
static const int wtLevel = 1;
static const int mmtLevel = 1;
static const int compassBins = 128;

static const std::string nameDim_0 = "Y";
static const std::string nameDim_1 = "X";
static const std::string nameAttr_0 = "ATTR_1";
static const core::eleType typeAttr_0 = core::eleType::UINT8;

std::pair<int, int> getParam(compressionType compType);
core::pArrayDesc getDummyArrayDesc();
std::shared_ptr<AFLOperator> getArrayBuildAFL(
	compressionType compType = compressionType::RAW);
}		// array_mem_char_4x4
}		// dummy
}		// msdb
#endif	// _MSDB_DUMMY_ARRAY_IN_MEMORY_H_