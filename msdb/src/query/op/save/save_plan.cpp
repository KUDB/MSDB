#include <pch.h>
#include <op/save/save_plan.h>
#include <op/save/save_action.h>

namespace msdb
{
namespace core
{
save_plan::save_plan()
{
}

save_plan::~save_plan()
{
}

const char* save_plan::name()
{
	return "save_plan";
}

pAction save_plan::makeAction()
{
	return std::make_shared<save_action>();
}

save_array_pset::save_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
save_plan_pset::save_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb