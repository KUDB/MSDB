#pragma once
#ifndef _MSDB_TEST_DUMMY_H_
#define _MSDB_TEST_DUMMY_H_
#include <pch_test.h>
#include <array/arrayDesc.h>

namespace msdb
{
namespace test
{
core::pDimensionDescs dimensionDescBuilder(
	std::vector<std::string> dimNames, core::dimension& dims, 
	core::dimension& chunkDims, core::dimension& blockDims
);
core::pAttributeDescs attributeDescBuilder(
	std::vector<std::string> attrNames,
	std::vector<core::eleType> attrTypes
);

core::pArrayDesc getDummyArrayDesc_SIMPLE_2D();
}		// test
}		// msdb
#endif	// _MSDB_TEST_DUMMY_H_