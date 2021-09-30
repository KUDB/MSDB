#include <dummy_array.h>

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
	core::dimensionId nums = core::dimensionId(dimNames.size());

	for (core::dimensionId id = 0; id < nums; ++id)
	{
		dimDescs->push_back(std::make_shared<core::dimensionDesc>(id, dimNames[id], 0, dims[id], chunkDims[id], blockDims[id]));
	}
	return dimDescs;
}
core::pAttributeDescs attributeDescBuilder(const std::vector<std::string> attrNames,
										   const std::vector<core::eleType> attrTypes)
{
	core::pAttributeDescs attrDescs = std::make_shared<core::attributeDescs>();
	core::attributeId nums = core::attributeId(attrNames.size());

	for (core::attributeId id = 0; id < nums; ++id)
	{
		attrDescs->push_back(std::make_shared<core::attributeDesc>(id, attrNames[id], attrTypes[id]));
	}
	return attrDescs;
}

std::pair<core::arrayId, std::string> getArrayIdName(compressionType compType, core::arrayId baseId, std::string baseName)
{
	switch (compType)
	{
	case compressionType::SEACOW:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_seacow, 
			baseName + "_" + compressionTypeToString(compressionType::SEACOW));
	case compressionType::SEACOW_HUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_seacow_huffman, 
			baseName + "_" + compressionTypeToString(compressionType::SEACOW_HUFFMAN));
	case compressionType::SPIHT:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_spiht, 
			baseName + "_" + compressionTypeToString(compressionType::SPIHT));
	case compressionType::COMPASS:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_compass, 
			baseName + "_" + compressionTypeToString(compressionType::COMPASS));
	case compressionType::LZW:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_lzw, 
			baseName + "_" + compressionTypeToString(compressionType::LZW));
	case compressionType::HUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_huffman,
			baseName + "_" + compressionTypeToString(compressionType::HUFFMAN));
	case compressionType::ADAPTHUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_adapt_huffman,
			baseName + "_" + compressionTypeToString(compressionType::ADAPTHUFFMAN));
	case compressionType::LZW_HUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_lzw_huffman, 
			baseName + "_" + compressionTypeToString(compressionType::LZW_HUFFMAN));
	case compressionType::ZIP:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_zip, 
			baseName + "_" + compressionTypeToString(compressionType::ZIP));
	}

	return std::make_pair(baseId, baseName);
}

Array getArrayAFL(std::string arrName, compressionType compType)
{
	auto idName = getArrayIdName(compType, 0, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	return Array(ctx, aname);
}

std::shared_ptr<AFLOperator> getInsertAFL(
	std::string arrName, std::string filePath,
	compressionType compType, int paramOne, int paramTwo)
{
	switch (compType)
	{
	case compressionType::NONE:
	case compressionType::RAW:
	{
		msdb::Context ctx;
		auto afl = msdb::Save(
			msdb::Insert(
			getArrayAFL(arrName), filePath)
		);
		return afl;
	}
	case compressionType::HUFFMAN:
	case compressionType::ADAPTHUFFMAN:
	case compressionType::LZW_HUFFMAN:
	case compressionType::LZW:
	case compressionType::ZIP:
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
			msdb::Insert(
			getArrayAFL(arrName, compType), filePath),
			compType
		);
		return afl;
	}
	case compressionType::SPIHT:
	case compressionType::COMPASS:
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
			msdb::Insert(
			getArrayAFL(arrName, compType), filePath),
			compType, paramOne
		);
		return afl;
	}
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
			msdb::Insert(
				getArrayAFL(arrName, compType), filePath),
				compType, paramOne, paramTwo
		);
		return afl;
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<AFLOperator> getLoadAFL(std::string arrName, compressionType compType, int paramOne, int paramTwo)
{
	switch (compType)
	{
	case compressionType::NONE:
	case compressionType::RAW:
	{
		msdb::Context ctx;
		auto afl = msdb::Load(
			getArrayAFL(arrName));
		return afl;
	}
	case compressionType::HUFFMAN:
	case compressionType::ADAPTHUFFMAN:
	case compressionType::LZW_HUFFMAN:
	case compressionType::LZW:
	case compressionType::ZIP:
	{
		msdb::Context ctx;
		auto afl = msdb::Decomp(
			getArrayAFL(arrName, compType),
			compType
		);
		return afl;
	}
	case compressionType::SPIHT:
	case compressionType::COMPASS:
	{
		msdb::Context ctx;
		auto afl = msdb::Decomp(
			getArrayAFL(arrName, compType),
			compType, paramOne
		);
		return afl;
	}
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		msdb::Context ctx;
		auto afl = msdb::Decomp(
			getArrayAFL(arrName, compType),
			compType, paramOne, paramTwo
		);
		return afl;
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<AFLOperator> getBuildIndexAFL(std::string arrName, std::string filePath, compressionType compType, attrIndexType idxType, int paramOne, int paramTwo)
{
	switch (idxType)
	{
	case attrIndexType::MMT:
	{
		msdb::Context ctx;
		auto afl = msdb::BuildIndex(
			msdb::Insert(
				getArrayAFL(arrName, compType), filePath),
			idxType, paramOne
		);
		return afl;
	}
	case attrIndexType::COMPASS:
	{
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<AFLOperator> getSaveIndexAFL(std::string arrName, compressionType compType, attrIndexType idxType, int paramOne, int paramTwo)
{
	switch (idxType)
	{
	case attrIndexType::MMT:
	{
		msdb::Context ctx;
		auto afl = msdb::SaveIndex(
			getArrayAFL(arrName, compType),
			idxType
		);
		return afl;
	}
	case attrIndexType::COMPASS:
	{
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<AFLOperator> getLoadIndexAFL(std::string arrName, compressionType compType, attrIndexType idxType, int paramOne, int paramTwo)
{
	switch (idxType)
	{
	case attrIndexType::MMT:
	{
		msdb::Context ctx;
		auto afl = msdb::LoadIndex(
			getArrayAFL(arrName, compType),
			idxType
		);
		return afl;
	}
	case attrIndexType::COMPASS:
	{
	}
	default:
		return nullptr;
	}
}


namespace data_star1024x1024
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
		return std::make_pair(compassBins, 0);
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
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(compressionType compType)
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
			msdb::DefAttribute(nameAttr_0, typeAttr_0, compType)
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_star1024x1024

namespace data_solar1024x1024
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
		return std::make_pair(compassBins, 0);
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
core::pArrayDesc getDummyArrayDesc()
{
// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(compressionType compType)
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
			msdb::DefAttribute(nameAttr_0, typeAttr_0, compType)
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_solar1024x1024

namespace data_saturn1024x1024
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
		return std::make_pair(compassBins, 0);
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
core::pArrayDesc getDummyArrayDesc()
{
// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(compressionType compType)
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
			msdb::DefAttribute(nameAttr_0, typeAttr_0, compType)
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_saturn1024x1024

namespace data_mars4096x2048
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
		return std::make_pair(compassBins, 0);
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
core::pArrayDesc getDummyArrayDesc()
{
// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(compressionType compType)
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
			msdb::DefAttribute(nameAttr_0, typeAttr_0, compType)
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_mars4096x2048

namespace data_mercury20480x10240
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
		return std::make_pair(compassBins, 0);
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
core::pArrayDesc getDummyArrayDesc()
{
// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(compressionType compType)
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
			msdb::DefAttribute(nameAttr_0, typeAttr_0, compType)
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_mercury20480x10240

namespace data_lunar102400x40960
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
		return std::make_pair(compassBins, 0);
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
core::pArrayDesc getDummyArrayDesc()
{
// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(compressionType compType)
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
			msdb::DefAttribute(nameAttr_0, typeAttr_0, compType)
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(compressionType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_lunar102400x40960
}		// dummy
}		// msdb