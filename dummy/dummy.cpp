#include "dummy.h"

namespace msdb
{
namespace dummy
{
core::pDimensionDescs dimensionDescBuilder(const std::vector<std::string> dimNames,
										   const core::dimension& dims,
										   const core::dimension& chunkDims,
										   const core::dimension& blockDims)
{
	core::pDimensionDescs dimDescs = std::make_shared<core::dimensionDescs>();
	core::dimensionId nums = dimNames.size();

	for (core::dimensionId id = 0; id < nums; ++id)
	{
		dimDescs->push_back(std::make_shared<core::dimensionDesc>(id, dimNames[id], 0, dims[id], chunkDims[id], blockDims[id]));
	}
	return dimDescs;
}
core::pAttributeDescs attributeDescBuilder(std::vector<std::string> attrNames,
										   std::vector<core::eleType> attrTypes)
{
	core::pAttributeDescs attrDescs = std::make_shared<core::attributeDescs>();
	core::attributeId nums = attrNames.size();

	for (core::attributeId id = 0; id < nums; ++id)
	{
		attrDescs->push_back(std::make_shared<core::attributeDesc>(id, attrNames[id], attrTypes[id]));
	}
	return attrDescs;
}

namespace data_star1024x1024
{
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ "ATTR_1" }, { core::eleType::UINT8 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}
}		// data_star1024x1024

namespace data_solar1024x1024
{
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ "ATTR_1" }, { core::eleType::UINT8 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}
}		// data_solar1024x1024

namespace data_saturn1024x1024
{
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ "ATTR_1" }, { core::eleType::UINT8 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}
}		// data_saturn1024x1024

namespace data_mars4096x2048
{
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ "ATTR_1" }, { core::eleType::UINT8 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}
}		// data_mars4096x2048

namespace data_mercury20480x10240
{
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ "ATTR_1" }, { core::eleType::UINT8 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}
}		// data_mercury20480x10240

namespace data_lunar102400x40960
{
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ "Y", "X" }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ "ATTR_1" }, { core::eleType::UINT16 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}
}		// data_lunar102400x40960
}		// dummy
}		// msdb
