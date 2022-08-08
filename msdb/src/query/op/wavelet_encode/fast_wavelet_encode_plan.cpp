#include <pch.h>
#include <op/wavelet_encode/fast_wavelet_encode_plan.h>
#include <op/wavelet_encode/fast_wavelet_encode_action.h>
#include <util/math.h>

namespace msdb
{
namespace core
{
fast_wavelet_encode_plan::fast_wavelet_encode_plan()
{
}

const char* msdb::core::fast_wavelet_encode_plan::name()
{
	return "fast_wavelet_encode";
}

//void fast_wavelet_encode_plan::initParamSets()
//{
//	this->addParamSet(std::make_shared<fast_wavelet_encode_array_pset>());
//}

pAction fast_wavelet_encode_plan::makeAction()
{
	return std::make_shared<fast_wavelet_encode_action>();
}

//////////////////////////////
// pset
fast_wavelet_encode_array_pset::fast_wavelet_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);		// Source array
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);		// Target wtLevel
}

pArrayDesc fast_wavelet_encode_array_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	for (dimensionId d = 0; d < aInferDesc->dimDescs_->size(); d++)
	{
		auto dDesc = aInferDesc->dimDescs_->at(d);
		dDesc->chunkSize_ = dDesc->blockSize_;
	}

	return aInferDesc;
}

fast_wavelet_encode_plan_pset::fast_wavelet_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);		// Target wtLevel
}
pArrayDesc fast_wavelet_encode_plan_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamPlan::paramType>(this->params_[0]->getParam())->inferSchema();
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	for (dimensionId d = 0; d < aInferDesc->dimDescs_->size(); d++)
	{
		auto dDesc = aInferDesc->dimDescs_->at(d);
		dDesc->chunkSize_ = dDesc->blockSize_;
	}

	return aInferDesc;
}

pBitmapTree fast_wavelet_encode_plan_pset::inferBottomUpBitmap()
{
	auto sourceBitmap = this->getSourcePlanBottomUpBitmap();
	//TODO::Implement inferBottomUpBitmap
	return sourceBitmap;
}
}		// core
}		// msdb