#include <dummy_array_in_memory.h>

namespace msdb
{
namespace dummy
{
namespace array_mem_twoattr_2d
{
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder(
		{ nameAttr_0, nameAttr_1}, { typeAttr_0, typeAttr_1});

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}
std::shared_ptr<AFLOperator> getArrayBuildAFL(materializedType matType, compressionType compType)
{
	auto idName = getArrayIdName(compType, aid, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	auto afl = msdb::Build(
		aid, aname,
		{
			msdb::DefDimension(nameDim_0, 0, dims[0], chunkDims[0], blockDims[0]),
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1])
		},
		{
			msdb::DefAttribute(nameAttr_0, typeAttr_0, matType, compType),
			msdb::DefAttribute(nameAttr_0, typeAttr_1, matType, compType)
		});
	return afl;
}
}	// array_mem_twoattr_2d
namespace array_mem_char_4x4
{
std::pair<int, int> getParam(compressionType compType)
{
	switch (compType)
	{
	case compressionType::NONE:
	case compressionType::RAW:
	case compressionType::HUFFMAN:
	case compressionType::ADAPTHUFFMAN:
	case compressionType::LZW_HUFFMAN:
	case compressionType::LZW:
	case compressionType::ZIP:
	{
		return std::make_pair(0, 0);
	}
	case compressionType::SPIHT:
	{
		return std::make_pair(wtLevel, 0);
	}
	case compressionType::COMPASS:
	{
		return std::make_pair(compassBins, compassBins);
	}
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		return std::make_pair(mmtLevel, wtLevel);
	}
	default:
		return std::make_pair(0, 0);
	}
}

core::pArrayDesc getDummyArrayDesc() {
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder(
		{ nameAttr_0, nameAttr_1, nameAttr_2 }, { typeAttr_0, typeAttr_1, typeAttr_2 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(materializedType matType, compressionType compType)
{
	auto idName = getArrayIdName(compType, aid, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	auto afl = msdb::Build(
		aid, aname,
		{
			msdb::DefDimension(nameDim_0, 0, dims[0], chunkDims[0], blockDims[0]),
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1])
		},
		{
			msdb::DefAttribute(nameAttr_0, typeAttr_0, matType, compType),
			msdb::DefAttribute(nameAttr_0, typeAttr_1, matType, compType),
			msdb::DefAttribute(nameAttr_0, typeAttr_2, matType, compType)
		});
	return afl;
}
}		// array_mem_char_4x4
}		// dummy
}		// msdb