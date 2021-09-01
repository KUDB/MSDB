#include <pch.h>
#include <op/load/load_plan.h>
#include <op/load/load_action.h>

namespace msdb
{
namespace core
{
load_plan::load_plan()
{
}

load_plan::~load_plan()
{
}

const char* load_plan::name()
{
	return "load_plan";
}

pAction load_plan::makeAction()
{
	return std::make_shared<load_action>();
}

load_array_pset::load_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
//load_plan_pset::load_plan_pset(parameters& pSet)
//	: opPlanParamSet(pSet)
//{
//	assert(this->params_.size() == 1);
//}
}		// core
}		// msdb