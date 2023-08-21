#include <dummy_array_in_memory.h>
#include <api_cpp_operator/cpp_io_operators.h>
#include <dummy_array_util.h>
#include <op/insert/insert_plan.h>

using namespace msdb::core;
using namespace msdb::dummy;

namespace msdb
{
namespace dummy
{
namespace array_uint8_2_16x16_4x4
{
Query::Status buildArray()
{
	auto qry = msdb::Query(getArrayBuildAFL());
	qry.unsetVerbose();
	qry.unsetRawResultOut();
	qry.execute();
	return qry.getStatus();
}
Query::Status insertTestData()
{
	msdb::Context ctx;

	auto attrs = generateTestData();
	auto insertAfl = msdb::Save(
		msdb::Insert(
			msdb::Array(ctx, array_uint8_2_16x16_4x4::arrName),
			attrs));
	auto qry = msdb::Query(insertAfl);
	qry.unsetRawResultOut();
	qry.execute();

	return qry.getStatus();
}
std::map<attributeId, msdb::core::opParamMemory> generateTestData()
{
	const uint64_t dataSize = 16 * 16;
	auto pDataA_0 = generateData<uint8_t, dataSize>([n = 0]() mutable {return n++; });
	auto pDataA_1 = generateData<uint8_t, dataSize>([n = 0]() mutable {return n++ * 2; });

	std::map<attributeId, msdb::core::opParamMemory> attrs;
	attrs[0] = msdb::core::opParamMemory(pDataA_0, pDataA_0->size());
	attrs[1] = msdb::core::opParamMemory(pDataA_1, pDataA_0->size());

	return attrs;
}
core::pArrayDesc getDummyArrayDesc()
{
	// Build dimension desc
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder(
		{ nameAttr_0, nameAttr_1}, { typeAttr_0, typeAttr_1});

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
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1])
		},
		{
			msdb::DefAttribute(nameAttr_0, typeAttr_0, matType, compType),
			msdb::DefAttribute(nameAttr_1, typeAttr_1, matType, compType)
		});
	return afl;
}
}	// array_uint8_2_16x16_4x4
namespace array_mem_char_4x4
{
Query::Status buildArray()
{
	auto qry = msdb::Query(getArrayBuildAFL());
	qry.unsetVerbose();
	qry.unsetRawResultOut();
	qry.execute();
	return qry.getStatus();
}

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
	core::pDimensionDescs dimDescs = dimensionDescBuilder({ nameDim_0, nameDim_1 }, dims, chunkDims, blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder(
		{ nameAttr_0, nameAttr_1, nameAttr_2 }, { typeAttr_0, typeAttr_1, typeAttr_2 });

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
			msdb::DefDimension(nameDim_1, 0, dims[1], chunkDims[1], blockDims[1])
		},
		{
			msdb::DefAttribute(nameAttr_0, typeAttr_0, matType, compType),
			msdb::DefAttribute(nameAttr_1, typeAttr_1, matType, compType),
			msdb::DefAttribute(nameAttr_2, typeAttr_2, matType, compType)
		});
	return afl;
}
}		// array_mem_char_4x4
}		// dummy
}		// msdb