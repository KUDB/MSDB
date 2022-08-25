#include <dummy_astronomy_array.h>
#include <op/wavelet_encode/wtChunk.h>
#include <index/mmt.h>
#include <index/compass.h>
#include <op/se_compression/seChunk.h>

namespace msdb
{
namespace dummy
{
//////////////////////////////
// Parameters
//////////////////////////////
namespace data_star1024x1024
{
//const core::dimension dims({ 16, 16 });
//const core::dimension chunkDims({ 16, 16 });
//const core::dimension blockDims({ 16, 16 });

const core::dimension dims({ 1024, 1024 });
const core::dimension chunkDims({ 64, 64 });
const core::dimension blockDims({ 64, 64 });

const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 128;
}

namespace data_saturn1024x1024
{
const core::dimension dims({ 1024, 1024 });
const core::dimension chunkDims({ 64, 64 });
const core::dimension blockDims({ 64, 64 });

const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 128;
}

namespace data_solar1024x1024
{
const core::dimension dims({ 4096, 4096 });
const core::dimension chunkDims({ 64, 64 });
const core::dimension blockDims({ 64, 64 });

const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 128;
}

namespace data_mars4096x2048
{
const core::dimension dims({ 2048, 4096 });
const core::dimension chunkDims({ 128, 128 });
const core::dimension blockDims({ 128, 128 });

const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 128;
}

namespace data_mercury20480x10240
{
//const core::dimension dims({ 10240, 20480 });
const core::dimension dims({ 8192, 16384 });
const core::dimension chunkDims({ 128, 128 });
const core::dimension blockDims({ 128, 128 });
//const core::dimension chunkDims({ 2048, 2048 });
//const core::dimension blockDims({ 2048, 2048 });

const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 128;
}

namespace data_lunar102400x40960
{
const core::dimension dims({ 32768, 98304 });
const core::dimension chunkDims({ 512, 512 });	// 64x192
const core::dimension blockDims({ 512, 512 });
//const core::dimension chunkDims({ 2048, 2048 });
//const core::dimension blockDims({ 2048, 2048 });

const int wtLevel = 3;
const int mmtLevel = 3;
const int compassBins = 256;
}

//////////////////////////////
// Functions
//////////////////////////////

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
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

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
std::shared_ptr<AFLOperator> getInsertSaveAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
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
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

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
std::shared_ptr<AFLOperator> getInsertSaveAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
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
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

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
std::shared_ptr<AFLOperator> getInsertSaveAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
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
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

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
std::shared_ptr<AFLOperator> getInsertSaveAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
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
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

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
std::shared_ptr<AFLOperator> getInsertSaveAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
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
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder({ nameAttr_0 }, { typeAttr_0 });

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
std::shared_ptr<AFLOperator> getInsertSaveAFL(compressionType compType)
{
	auto param = getParam(compType);
	return dummy::getInsertSaveAFL(arrName, filePath, compType, param.first, param.second);
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