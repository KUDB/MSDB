#include <pch.h>
#include <op/sz3_encode/sz3_encode_plan.h>
#include <op/sz3_encode/sz3_encode_action.h>

namespace msdb
{
namespace core
{
sz3_encode_plan::sz3_encode_plan()
{
}
const char* sz3_encode_plan::name()
{
	return "sz3_encode_plan";
}
pAction sz3_encode_plan::makeAction()
{
	return std::make_shared<sz3_encode_action>();
}

//////////////////////////////
// pset
sz3_encode_array_pset::sz3_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
sz3_encode_plan_pset::sz3_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb