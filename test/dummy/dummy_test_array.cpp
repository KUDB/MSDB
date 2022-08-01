#include <pch_test.h>
#include <dummy/dummy_test_array.h>
#include <dummy_array_util.h>
#include <op/wavelet_encode/wtChunk.h>
#include <index/mmt.h>
namespace msdb
{
namespace test
{
core::pArrayDesc getDummyArrayDesc_SIMPLE_2D()
{
	// Dummy array descriptions
	core::arrayId aid = 100;
	std::string arrayName = "SIMPLE_2D";
	core::dimension dims({ 1024, 1024 });
	core::dimension chunkDims({ 128, 128 });
	core::dimension blockDims({ 32, 32 });

	// Build dimension desc
	core::pDimensionDescs dimDescs = dummy::dimensionDescBuilder({"Y", "X"}, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = dummy::attributeDescBuilder({ "ATTR_1" }, {core::concreteTy<char>()});

	return std::make_shared<core::arrayDesc>(aid, arrayName.c_str(), dimDescs, attrDescs);
}

core::pArrayDesc getDummyArrayDesc_MultiAttr_2D()
{
	// Dummy array descriptions
	core::arrayId aid = 102;
	std::string arrayName = "MULTIATTR_2D";
	core::dimension dims({ 128, 128});
	core::dimension chunkDims({ 32, 32});
	core::dimension blockDims({ 4, 4});

	// Build dimension desc
	core::pDimensionDescs dimDescs = dummy::dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = dummy::attributeDescBuilder(
		{ "ATTR_1", "ATTR_2"},
		{ core::concreteTy<char>(), core::concreteTy<int16_t>() });

	return std::make_shared<core::arrayDesc>(aid, arrayName.c_str(), dimDescs, attrDescs);
}

core::pArrayDesc getDummyArrayDesc_MultiAttr_3D()
{
	// Dummy array descriptions
	core::arrayId aid = 112;
	std::string arrayName = "MULTIATTR_3D";
	core::dimension dims({ 128, 128, 128 });
	core::dimension chunkDims({32, 32, 32});
	core::dimension blockDims({16, 16, 16});

	size_t waveletLevel = 3;
	size_t mmtLevel = 2;

	// Build dimension desc
	core::pDimensionDescs dimDescs = dummy::dimensionDescBuilder({ "Z", "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = dummy::attributeDescBuilder(
		{ "ATTR_1", "ATTR_2", "ATTR_3" },
		{ core::concreteTy<char>(), core::concreteTy<int8_t>(), core::concreteTy<float>() },
		{ core::materializedType::FLATTEN, core::materializedType::NESTED , core::materializedType::FLATTEN },
		{ core::compressionType::NONE, core::compressionType::SEACOW, core::compressionType::RAW },
		{
			core::attributeDesc::paramType({}),
			core::attributeDesc::paramType(
				{
					std::make_pair<>(_STR_PARAM_WAVELET_LEVEL_, std::to_string(waveletLevel)),
					std::make_pair<>(_STR_PARAM_MMT_LEVEL_, std::to_string(mmtLevel))
				}),
			core::attributeDesc::paramType({})
		});

	return std::make_shared<core::arrayDesc>(aid, arrayName.c_str(), dimDescs, attrDescs);
}
}		// test
}		// msdb