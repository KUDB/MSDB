#include <dummy_array.h>
#include <api_cpp/msdb.h>
#include <system/exceptions.h>
#include <op/insert/insert_plan.h>
#include <api_cpp_operator/cpp_io_operators.h>

namespace msdb
{
namespace dummy
{
Array dummy_array::getArrayAFL(std::string arrName, encodingType compType)
{
	auto idName = getArrayIdName(compType, 0, arrName);
	core::arrayId aid = idName.first;
	std::string aname = idName.second;

	msdb::Context ctx;
	return Array(ctx, aname);
}
std::shared_ptr<AFLOperator> dummy_array::getInsertSaveAFL(std::string arrName, std::string filePath, encodingType compType, int paramOne, int paramTwo)
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
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED, "Invalid EncodingType, getInsertAFL() not supports "));
		return nullptr;
	}
}
std::shared_ptr<AFLOperator> dummy_array::getLoadAFL(std::string arrName, encodingType compType, int paramOne, int paramTwo)
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
std::shared_ptr<AFLOperator> dummy_array::getBuildIndexAFL(std::string arrName, std::string filePath, encodingType compType, attrIndexType idxType, int paramOne, int paramTwo)
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
std::shared_ptr<AFLOperator> dummy_array::getSaveIndexAFL(std::string arrName, encodingType compType, attrIndexType idxType, int paramOne, int paramTwo)
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
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED, 
			"An Operator to save COMPASS Index is not implemented"));
	}
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED, 
			"Unknown Index - dummy_array::getSaveIndexAFL"));
		return nullptr;
	}
}
std::shared_ptr<AFLOperator> dummy_array::getLoadIndexAFL(std::string arrName, encodingType compType, attrIndexType idxType, int paramOne, int paramTwo)
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
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED, 
			"An Operator to load COMPASS Index is not implemented"));
	}
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED, 
			"Unknown Index - dummy_array::getLoadIndexAFL"));
		return nullptr;
	}
}
std::vector<DefDimension> dummy_array::getDefDimensions()
{
	int dimSize = this->_dims.size();
	if (dimSize != this->_nameDim.size() || dimSize != this->_chunkDims.size() || dimSize != this->_blockDims.size())
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT, 
			"The dimension size is different."));

	std::vector<DefDimension> out;
	for (int i = 0; i < this->_dims.size(); ++i)
	{
		out.push_back(msdb::DefDimension(this->_nameDim[i], 0, this->_dims[i], this->_chunkDims[i], this->_blockDims[i]));
	}
	return out;
}
std::vector<DefAttribute> dummy_array::getDefAttributes()
{
	int numAttr = this->_nameAttr.size();
	if(numAttr != this->_typeAttr.size() || numAttr != this->_matTypes.size() || numAttr != this->_compTypes.size())
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT, 
			"The number of attributes is different."));

	std::vector<DefAttribute> out;
	for (int i = 0; i < numAttr; ++i)
	{
		out.push_back(msdb::DefAttribute(this->_nameAttr[i], this->_typeAttr[i], this->_matTypes[i], this->_compTypes[i]));
	}
	return out;
}
core::pArrayDesc dummy_array::getDummyArrayDesc()
{
	core::pDimensionDescs dimDescs = dimensionDescBuilder(this->_nameDim, this->_dims, this->_chunkDims, this->_blockDims);
	core::pAttributeDescs attrDescs = attributeDescBuilder(this->_nameAttr, this->_typeAttr, this->_matTypes, this->_compTypes, this->_attrParams);

	return std::make_shared<core::arrayDesc>(this->_aid, this->_arrName, dimDescs, attrDescs);
}
dummy_array::dummy_array(const std::string arrName, const core::arrayId aid,
	const core::dimension& dims, const core::dimension& chunkDims, const core::dimension& blockDims,
	const std::vector<std::string>& nameDim, const std::vector<std::string>& nameAttr,
	const std::vector<core::dataType>& typeAttr,
	const std::vector<core::materializedType>& matTypes,
	const std::vector<core::encodingType>& compTypes,
	const std::vector<std::map<std::string, std::string>>& attrParams)
	: _arrName(arrName), _aid(aid), 
	_dims(dims), _chunkDims(chunkDims), _blockDims(blockDims), 
	_nameDim(nameDim), _nameAttr(nameAttr), _typeAttr(typeAttr), 
	_matTypes(matTypes), _compTypes(compTypes), _attrParams(attrParams)
{
	if (dims.size() != chunkDims.size() || dims.size() != blockDims.size())
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT, 
			"dummy_array::Array dimension size should be equal with chunk or block dimension size."));

	if(nameAttr.size() != typeAttr.size())
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_WRONG_INPUT, 
			"dummy_array::Number of attribute in nameAttr and typeAttr should be equal."))

	////////////////////////
	// Fill the member varialbes with default parameters
	if (matTypes.size() < nameAttr.size())
	{
		int diff = int(nameAttr.size()) - matTypes.size();
		for (int i = 0; i < diff; ++i)
		{
			this->_matTypes.push_back(core::materializedType::FLATTEN);
		}
	}

	if (compTypes.size() < nameAttr.size())
	{
		int diff = int(nameAttr.size()) - compTypes.size();
		for (int i = 0; i < diff; ++i)
		{
			this->_compTypes.push_back(core::encodingType::NONE);
		}
	}

	if (attrParams.size() < nameAttr.size())
	{
		int diff = int(nameAttr.size()) - attrParams.size();
		for (int i = 0; i < diff; ++i)
		{
			this->_attrParams.push_back({});
		}
	}
	////////////////////////
}
}	// dummy
}	// msdb

