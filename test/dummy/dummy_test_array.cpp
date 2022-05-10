#include <pch_test.h>
#include <dummy/dummy_test_array.h>
#include <dummy_array_util.h>

namespace msdb
{
namespace test
{
//core::pDimensionDescs dimensionDescBuilder(std::vector<std::string> dimNames, 
//										   core::dimension& dims, 
//										   core::dimension& chunkDims, 
//										   core::dimension& blockDims)
//{
//	core::pDimensionDescs dimDescs = std::make_shared<core::dimensionDescs>();
//	core::dimensionId nums = dimNames.size();
//
//	for (core::dimensionId id = 0; id < nums; ++id)
//	{
//		dimDescs->push_back(std::make_shared<core::dimensionDesc>(id, dimNames[id], 0, dims[id], chunkDims[id], blockDims[id]));
//	}
//	return dimDescs;
//}
//core::pAttributeDescs attributeDescBuilder(std::vector<std::string> attrNames,
//										   std::vector<core::dataType> attrTypes)
//{
//	core::pAttributeDescs attrDescs = std::make_shared<core::attributeDescs>();
//	core::attributeId nums = attrNames.size();
//
//	for (core::attributeId id = 0; id < nums; ++id)
//	{
//		attrDescs->push_back(std::make_shared<core::attributeDesc>(id, attrNames[id], attrTypes[id]));
//	}
//	return attrDescs;
//}
core::pArrayDesc getDummyArrayDesc_SIMPLE_2D()
{
	// Dummy array descriptions
	core::arrayId aid = 100;
	std::string arrayName = "SIMPLE_2D";
	core::dimension dims(1024, 1024);
	core::dimension chunkDims(128, 128);
	core::dimension blockDims(32, 32);

	// Build dimension desc
	core::pDimensionDescs dimDescs = dummy::dimensionDescBuilder({"Y", "X"}, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = dummy::attributeDescBuilder({ "ATTR_1" }, {core::concreteTy<char>()});

	return std::make_shared<core::arrayDesc>(aid, arrayName.c_str(), dimDescs, attrDescs);
}
}		// test
}		// msdb