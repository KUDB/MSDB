#include "dummy_astronomy_array_3d.h"
#include <op/wavelet_encode/wtChunk.h>
#include <index/mmt.h>
#include <index/compass.h>
#include <op/seacow/seChunk.h>
#include <api_cpp_operator/cpp_io_operators.h>

namespace msdb
{
namespace dummy
{
namespace data_star64x64x64
{
std::pair<int, int> getParam(encodingType compType)
{
	switch (compType)
	{
	case encodingType::NONE:
	case encodingType::RAW:
	case encodingType::HUFFMAN:
	case encodingType::ADAPTHUFFMAN:
	case encodingType::LZW_HUFFMAN:
	case encodingType::LZW:
	case encodingType::ZIP:
	{
		return std::make_pair(0, 0);
	}
	case encodingType::SPIHT:
	{
		return std::make_pair(wtLevel, 0);
	}
	case encodingType::COMPASS:
	{
		return std::make_pair(compassBins, compassBins);
	}
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return std::make_pair(mmtLevel, wtLevel);
	}
	default:
		return std::make_pair(0, 0);
	}
}
core::pArrayDesc getDummyArrayDesc() {
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1, nameDim_2 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(materializedType matType, encodingType compType)
{
	auto idName = getArrayIdName(compType, aid, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	auto afl = msdb::Build(
		aid, aname,
		{
			msdb::DefDimension(nameDim_0, 0, dims[0], chunkDims[0], blockDims[0]),
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1]),
			msdb::DefDimension(nameDim_2, 0, dims[2], chunkDims[2], blockDims[2])
		},
		{
			msdb::DefAttribute(
				nameAttr_0, typeAttr_0, matType, compType,
				{
					std::make_pair<>(_STR_PARAM_WAVELET_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_SE_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_MMT_LEVEL_, std::to_string(mmtLevel)),
					std::make_pair<>(_STR_PARAM_COMPASS_BINS_, std::to_string(compassBins))
				})
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertSaveAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_star1024x1024

namespace data_nexrad_16x1024x2048
{
std::pair<int, int> getParam(encodingType compType)
{
	switch (compType)
	{
	case encodingType::NONE:
	case encodingType::RAW:
	case encodingType::HUFFMAN:
	case encodingType::ADAPTHUFFMAN:
	case encodingType::LZW_HUFFMAN:
	case encodingType::LZW:
	case encodingType::ZIP:
	{
		return std::make_pair(0, 0);
	}
	case encodingType::SPIHT:
	{
		return std::make_pair(wtLevel, 0);
	}
	case encodingType::COMPASS:
	{
		return std::make_pair(compassBins, compassBins);
	}
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return std::make_pair(mmtLevel, wtLevel);
	}
	default:
		return std::make_pair(0, 0);
	}
}
core::pArrayDesc getDummyArrayDesc() {
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1, nameDim_2 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(materializedType matType, encodingType compType)
{
	auto idName = getArrayIdName(compType, aid, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	auto afl = msdb::Build(
		aid, aname,
		{
			msdb::DefDimension(nameDim_0, 0, dims[0], chunkDims[0], blockDims[0]),
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1]),
			msdb::DefDimension(nameDim_2, 0, dims[2], chunkDims[2], blockDims[2])
		},
		{
			msdb::DefAttribute(
				nameAttr_0, typeAttr_0, matType, compType,
				{
					std::make_pair<>(_STR_PARAM_WAVELET_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_SE_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_MMT_LEVEL_, std::to_string(mmtLevel)),
					std::make_pair<>(_STR_PARAM_COMPASS_BINS_, std::to_string(compassBins))
				})
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertSaveAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_nexrad_16x1024x2048

namespace data_solar_sdo_1024x1024x512
{
std::pair<int, int> getParam(encodingType compType)
{
	switch (compType)
	{
	case encodingType::NONE:
	case encodingType::RAW:
	case encodingType::HUFFMAN:
	case encodingType::ADAPTHUFFMAN:
	case encodingType::LZW_HUFFMAN:
	case encodingType::LZW:
	case encodingType::ZIP:
	{
		return std::make_pair(0, 0);
	}
	case encodingType::SPIHT:
	{
		return std::make_pair(wtLevel, 0);
	}
	case encodingType::COMPASS:
	{
		return std::make_pair(compassBins, compassBins);
	}
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return std::make_pair(mmtLevel, wtLevel);
	}
	default:
		return std::make_pair(0, 0);
	}
}
core::pArrayDesc getDummyArrayDesc() {
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1, nameDim_2 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(materializedType matType, encodingType compType)
{
	auto idName = getArrayIdName(compType, aid, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	auto afl = msdb::Build(
		aid, aname,
		{
			msdb::DefDimension(nameDim_0, 0, dims[0], chunkDims[0], blockDims[0]),
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1]),
			msdb::DefDimension(nameDim_2, 0, dims[2], chunkDims[2], blockDims[2])
		},
		{
			msdb::DefAttribute(
				nameAttr_0, typeAttr_0, matType, compType,
				{
					std::make_pair<>(_STR_PARAM_WAVELET_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_SE_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_MMT_LEVEL_, std::to_string(mmtLevel)),
					std::make_pair<>(_STR_PARAM_COMPASS_BINS_, std::to_string(compassBins))
				})
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertSaveAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_solar_sdo_1024x1024x512

namespace data_nexrad_201711_64x24x1024x2048
{
std::pair<int, int> getParam(encodingType compType)
{
	switch (compType)
	{
	case encodingType::NONE:
	case encodingType::RAW:
	case encodingType::HUFFMAN:
	case encodingType::ADAPTHUFFMAN:
	case encodingType::LZW_HUFFMAN:
	case encodingType::LZW:
	case encodingType::ZIP:
	{
		return std::make_pair(0, 0);
	}
	case encodingType::SPIHT:
	{
		return std::make_pair(wtLevel, 0);
	}
	case encodingType::COMPASS:
	{
		return std::make_pair(compassBins, compassBins);
	}
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return std::make_pair(mmtLevel, wtLevel);
	}
	default:
		return std::make_pair(0, 0);
	}
}
core::pArrayDesc getDummyArrayDesc() {
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1, nameDim_2, nameDim_3 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

	return std::make_shared<core::arrayDesc>(aid, arrName.c_str(), dimDescs, attrDescs);
}

std::shared_ptr<AFLOperator> getArrayBuildAFL(materializedType matType, encodingType compType)
{
	auto idName = getArrayIdName(compType, aid, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	auto afl = msdb::Build(
		aid, aname,
		{
			msdb::DefDimension(nameDim_0, 0, dims[0], chunkDims[0], blockDims[0]),
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1]),
			msdb::DefDimension(nameDim_2, 0, dims[2], chunkDims[2], blockDims[2]),
			msdb::DefDimension(nameDim_3, 0, dims[3], chunkDims[3], blockDims[3])
		},
		{
			msdb::DefAttribute(
				nameAttr_0, typeAttr_0, matType, compType,
				{
					std::make_pair<>(_STR_PARAM_WAVELET_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_SE_LEVEL_, std::to_string(wtLevel)),
					std::make_pair<>(_STR_PARAM_MMT_LEVEL_, std::to_string(mmtLevel)),
					std::make_pair<>(_STR_PARAM_COMPASS_BINS_, std::to_string(compassBins))
				})
		});
	return afl;
}
std::shared_ptr<AFLOperator> getInsertSaveAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadAFL(encodingType compType)
{
	auto param = getParam(compType);
	return dummy::getLoadAFL(arrName, compType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getBuildIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getBuildIndexAFL(arrName, filePath, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getSaveIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getSaveIndexAFL(arrName, compType, idxType, param.first, param.second);
}
std::shared_ptr<AFLOperator> getLoadIndexAFL(encodingType compType, attrIndexType idxType)
{
	auto param = getParam(compType);
	return dummy::getLoadIndexAFL(arrName, compType, idxType, param.first, param.second);
}
}		// data_nexrad_201711_64x24x1024x2048
}		// dummy
}		// msdb