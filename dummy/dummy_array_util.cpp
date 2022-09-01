#include <dummy_array_util.h>
#include <system/exceptions.h>

namespace msdb
{
namespace dummy
{
using namespace msdb::core;

msdb::Array getArrayAFL(std::string arrName, encodingType compType)
{
	auto idName = getArrayIdName(compType, 0, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	return Array(ctx, aname);
}

std::shared_ptr<AFLOperator> getInsertSaveAFL(
	std::string arrName, std::string filePath,
	encodingType compType, int paramOne, int paramTwo)
{
	switch (compType)
	{
	case encodingType::NONE:
	case encodingType::RAW:
	{
		msdb::Context ctx;
		auto afl = msdb::Save(
			msdb::Insert(
				getArrayAFL(arrName), filePath)
		);
		return afl;
	}
	case encodingType::HUFFMAN:
	case encodingType::ADAPTHUFFMAN:
	case encodingType::LZW_HUFFMAN:
	case encodingType::LZW:
	case encodingType::ZIP:
	case encodingType::TTHRESH:
	case encodingType::ZFP:
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
			msdb::Insert(
				getArrayAFL(arrName, compType), filePath),
			compType
		);
		return afl;
	}
	case encodingType::SPIHT:
	case encodingType::COMPASS:
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
			msdb::Insert(
				getArrayAFL(arrName, compType), filePath),
			compType, paramOne
		);
		return afl;
	}
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
				msdb::Insert(
					getArrayAFL(arrName, compType), filePath),
			compType, paramOne, paramTwo);
		return afl;
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<AFLOperator> getLoadAFL(std::string arrName, encodingType compType, int paramOne, int paramTwo)
{
	switch (compType)
	{
	case encodingType::NONE:
	case encodingType::RAW:
	{
		msdb::Context ctx;
		auto afl = msdb::Load(
			getArrayAFL(arrName));
		return afl;
	}
	case encodingType::HUFFMAN:
	case encodingType::ADAPTHUFFMAN:
	case encodingType::LZW_HUFFMAN:
	case encodingType::LZW:
	case encodingType::ZIP:
	case encodingType::TTHRESH:
	case encodingType::ZFP:
	{
		msdb::Context ctx;
		auto afl = msdb::Decomp(
			getArrayAFL(arrName, compType),
			compType
		);
		return afl;
	}
	case encodingType::SPIHT:
	case encodingType::COMPASS:
	{
		msdb::Context ctx;
		auto afl = msdb::Decomp(
			getArrayAFL(arrName, compType),
			compType, paramOne
		);
		return afl;
	}
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
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

std::shared_ptr<AFLOperator> getBuildIndexAFL(std::string arrName, std::string filePath, encodingType compType, attrIndexType idxType, int paramOne, int paramTwo)
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
		msdb::Context ctx;
		auto afl = msdb::BuildIndex(
			msdb::Insert(
				getArrayAFL(arrName, compType), filePath),
			idxType, paramOne
		);
		return afl;
	}
	default:
		return nullptr;
	}
}

std::shared_ptr<AFLOperator> getSaveIndexAFL(std::string arrName, encodingType compType, attrIndexType idxType, int paramOne, int paramTwo)
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

std::shared_ptr<AFLOperator> getLoadIndexAFL(std::string arrName, encodingType compType, attrIndexType idxType, int paramOne, int paramTwo)
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
core::pAttributeDescs attributeDescBuilder(const std::vector<std::string>& attrNames,
										   const std::vector<core::dataType>& attrTypes)
{
	if (attrNames.size() != attrTypes.size())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_ATTRIBUTE_DESC, "Number of Attributes and their types are not matched"));
	}

	core::pAttributeDescs attrDescs = std::make_shared<core::attributeDescs>();
	core::attributeId nums = core::attributeId(attrNames.size());

	for (core::attributeId id = 0; id < nums; ++id)
	{
		attrDescs->push_back(std::make_shared<core::attributeDesc>(id, attrNames[id], attrTypes[id]));
	}
	return attrDescs;
}

core::pAttributeDescs attributeDescBuilder(const std::vector<std::string>& attrNames,
										   const std::vector<core::dataType>& attrTypes,
										   const std::vector<core::materializedType>& matTypes,
										   const std::vector<core::encodingType>& compTypes,
										   const std::vector<core::attributeDesc::paramType>& optionalParams)
{
	if (attrNames.size() != attrTypes.size())
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_ATTRIBUTE_DESC, "Number of Attributes and their types are not matched"));
	}

	core::pAttributeDescs attrDescs = std::make_shared<core::attributeDescs>();
	core::attributeId nums = core::attributeId(attrNames.size());

	for (core::attributeId id = 0; id < nums; ++id)
	{
		attrDescs->push_back(std::make_shared<core::attributeDesc>(id, attrNames[id], attrTypes[id], matTypes[id], compTypes[id], optionalParams[id]));
	}
	return attrDescs;
}

std::pair<core::arrayId, std::string> getArrayIdName(encodingType compType, core::arrayId baseId, std::string baseName)
{
	switch (compType)
	{
	case encodingType::SEACOW:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_seacow,
			baseName + "_" + encodingTypeToString(encodingType::SEACOW));
	case encodingType::SEACOW_HUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_seacow_huffman,
			baseName + "_" + encodingTypeToString(encodingType::SEACOW_HUFFMAN));
	case encodingType::SPIHT:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_spiht,
			baseName + "_" + encodingTypeToString(encodingType::SPIHT));
	case encodingType::COMPASS:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_compass,
			baseName + "_" + encodingTypeToString(encodingType::COMPASS));
	case encodingType::LZW:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_lzw,
			baseName + "_" + encodingTypeToString(encodingType::LZW));
	case encodingType::HUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_huffman,
			baseName + "_" + encodingTypeToString(encodingType::HUFFMAN));
	case encodingType::ADAPTHUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_adapt_huffman,
			baseName + "_" + encodingTypeToString(encodingType::ADAPTHUFFMAN));
	case encodingType::LZW_HUFFMAN:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_lzw_huffman,
			baseName + "_" + encodingTypeToString(encodingType::LZW_HUFFMAN));
	case encodingType::ZIP:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_zip,
			baseName + "_" + encodingTypeToString(encodingType::ZIP));
	case encodingType::TTHRESH:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_tthresh,
			baseName + "_" + encodingTypeToString(encodingType::TTHRESH));
	case encodingType::ZFP:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_zfp,
			baseName + "_" + encodingTypeToString(encodingType::ZFP));
	}

	return std::make_pair(baseId, baseName);
}
}		// dummy
}		// msdb