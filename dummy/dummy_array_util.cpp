#include <dummy_array_util.h>
#include <system/exceptions.h>

namespace msdb
{
namespace dummy
{
using namespace msdb::core;

msdb::Array getArrayAFL(std::string arrName, compressionType compType)
{
	auto idName = getArrayIdName(compType, 0, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	return Array(ctx, aname);
}

std::shared_ptr<AFLOperator> getInsertSaveAFL(
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
	//case compressionType::HUFFMAN:
	//case compressionType::ADAPTHUFFMAN:
	//case compressionType::LZW_HUFFMAN:
	//case compressionType::LZW:
	case compressionType::ZIP:
	case compressionType::TTHRESH:
	case compressionType::ZFP:
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
			msdb::Insert(
				getArrayAFL(arrName, compType), filePath),
			compType
		);
		return afl;
	}
	//case compressionType::SPIHT:
	//case compressionType::COMPASS:
	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::Comp(
	//		msdb::Insert(
	//			getArrayAFL(arrName, compType), filePath),
	//		compType, paramOne
	//	);
	//	return afl;
	//}
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		msdb::Context ctx;
		//auto afl = msdb::Comp(
		//	msdb::Between(
		//		msdb::Insert(
		//			getArrayAFL(arrName, compType), filePath),
		//			msdb::Domain(msdb::Coordinates({ 0, 512 }), msdb::Coordinates({64, 576}))
		//		),
		//	compType, paramOne, paramTwo);
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
	//case compressionType::HUFFMAN:
	//case compressionType::ADAPTHUFFMAN:
	//case compressionType::LZW_HUFFMAN:
	//case compressionType::LZW:
	case compressionType::ZIP:
	case compressionType::TTHRESH:
	case compressionType::ZFP:
	{
		msdb::Context ctx;
		auto afl = msdb::Decomp(
			getArrayAFL(arrName, compType),
			compType
		);
		return afl;
	}
	//case compressionType::SPIHT:
	//case compressionType::COMPASS:
	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::Decomp(
	//		getArrayAFL(arrName, compType),
	//		compType, paramOne
	//	);
	//	return afl;
	//}
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
	//case attrIndexType::COMPASS:
	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::BuildIndex(
	//		msdb::Insert(
	//			getArrayAFL(arrName, compType), filePath),
	//		idxType, paramOne
	//	);
	//	return afl;
	//}
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
										   const std::vector<core::compressionType>& compTypes,
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
	case compressionType::TTHRESH:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_tthresh,
			baseName + "_" + compressionTypeToString(compressionType::TTHRESH));
	case compressionType::ZFP:
		return std::make_pair<core::arrayId, std::string>(
			baseId + msdb::dummy::arr_id_zfp,
			baseName + "_" + compressionTypeToString(compressionType::ZFP));
	}

	return std::make_pair(baseId, baseName);
}
}		// dummy
}		// msdb