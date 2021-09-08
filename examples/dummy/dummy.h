#pragma once
#include <vector>
#include <string>
#include <array/arrayDesc.h>

#ifndef _MSDB_EXAMPLE_DUMMY_H_
#define _MSDB_EXAMPLE_DUMMY_H_

#define DUMMY_PATH(path) "..\\dummy\\"#path


namespace msdb
{
namespace star1024x1024
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
}		// star1024x1024
}		// msdb

#endif	// _MSDB_EXAMPLE_DUMMY_H_