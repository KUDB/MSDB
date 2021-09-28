#include <pch.h>
#include <op/compass_encode/compass_encode_plan.h>
#include <op/compass_encode/compass_encode_action.h>

namespace msdb
{
namespace core
{
compass_encode_plan::compass_encode_plan()
{
}
const char* compass_encode_plan::name()
{
	return "compass_encode_plan";
}
pAction compass_encode_plan::makeAction()
{
	return std::make_shared<compass_encode_action>();
}

//////////////////////////////
// ParamSets
compass_encode_array_pset::compass_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);	// Target Bin Numbers
}

compass_encode_plan_pset::compass_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);	// Target Bin Numbers
}
}		// core
}		// msdb