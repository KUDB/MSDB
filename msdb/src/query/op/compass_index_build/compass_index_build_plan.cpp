#include <pch.h>
#include <op/compass_index_build/compass_index_build_plan.h>
#include <op/compass_index_build/compass_index_build_action.h>
#include "compass_index_build_plan.h"

namespace msdb
{
namespace core
{
compass_index_build_plan::compass_index_build_plan()
{
}

const char* compass_index_build_plan::name()
{
	return "compass_index_build_plan";
}

pAction compass_index_build_plan::makeAction()
{
	return std::make_shared<compass_index_build_action>();
}

compass_index_build_array_pset::compass_index_build_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);	// Target bin nums
}

pArrayDesc compass_index_build_array_pset::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	pArrayDesc aInferDesc = std::make_shared<opParamArray::paramType>(*aSourceDesc);

	return aInferDesc;
}
}		// core
}		// msdb