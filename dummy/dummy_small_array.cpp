#include "dummy_small_array.h"
#include <api_cpp_operator/cpp_io_operators.h>

namespace msdb
{
namespace dummy
{
array_8x8::array_8x8(const std::string arrName, const core::arrayId aid,
	const core::dimension& chunkDims, const core::dimension& blockDims,
	const std::vector<std::string>& nameDim, const std::vector<std::string>& nameAttr,
	const std::vector<core::dataType>& typeAttr,
	const std::vector<core::materializedType>& matTypes,
	const std::vector<core::encodingType>& compTypes,
	const std::vector<std::map<std::string, std::string>>& attrParams)
	: dummy_array(arrName, aid, core::dimension({8, 8}), chunkDims, blockDims, nameDim, nameAttr, typeAttr, matTypes, compTypes, attrParams)
{
	assert(nameDim.size() == data_8x8::numDims);
}

core::attributeDesc::paramType array_8x8::getAttrParam(core::attributeId attrId)
{
	assert(attrId == 0);

	core::attributeDesc::paramType param = {
		core::make_attr_param(_STR_PARAM_WAVELET_LEVEL_, std::to_string(data_8x8::wtLevel)),
		core::make_attr_param(_STR_PARAM_SE_LEVEL_, std::to_string(data_8x8::wtLevel)),
		core::make_attr_param(_STR_PARAM_MMT_LEVEL_, std::to_string(data_8x8::mmtLevel)),
		core::make_attr_param(_STR_PARAM_COMPASS_BINS_, std::to_string(data_8x8::compassBins))
	};
	return param;
}
std::shared_ptr<AFLOperator> array_8x8::getArrayBuildAFL(
	std::vector<materializedType> matTypes, std::vector<encodingType> compType)
{
	if(!matTypes.empty())
		this->_matTypes = matTypes;
	if(!compType.empty())
		this->_compTypes = compType;

	msdb::Context ctx;
	return Build(this->_aid, this->_arrName, this->getDefDimensions(), this->getDefAttributes());
}
//////////////////
array_1x32x32::array_1x32x32(const std::string arrName, const core::arrayId aid,
	const std::vector<std::map<std::string, std::string>>& attrParams)
	: dummy_array(arrName, aid, core::dimension({ 1, 32, 32 }), core::dimension({ 1, 32, 32 }), core::dimension({ 1, 32, 32 }), 
		std::vector<std::string>({"id", "y", "x"}), 
		{ "img" }, { core::concreteTy<uint8_t>() }, { core::materializedType::FLATTEN }, { core::encodingType::NONE }, 
		attrParams)
{}
core::attributeDesc::paramType array_1x32x32::getAttrParam(core::attributeId attrId)
{
	assert(attrId == 0);

	core::attributeDesc::paramType param = {
		core::make_attr_param(_STR_PARAM_WAVELET_LEVEL_, std::to_string(data_1x32x32::wtLevel)),
		core::make_attr_param(_STR_PARAM_SE_LEVEL_, std::to_string(data_1x32x32::wtLevel)),
		core::make_attr_param(_STR_PARAM_MMT_LEVEL_, std::to_string(data_1x32x32::mmtLevel)),
		core::make_attr_param(_STR_PARAM_COMPASS_BINS_, std::to_string(data_1x32x32::compassBins))
	};
	return param;
}
std::shared_ptr<AFLOperator> array_1x32x32::getArrayBuildAFL(
	std::vector<materializedType> matTypes, std::vector<encodingType> compType)
{
	if (!matTypes.empty())
		this->_matTypes = matTypes;
	if (!compType.empty())
		this->_compTypes = compType;

	msdb::Context ctx;
	return Build(this->_aid, this->_arrName, this->getDefDimensions(), this->getDefAttributes());
}
}		// dummy
}		// msdb
