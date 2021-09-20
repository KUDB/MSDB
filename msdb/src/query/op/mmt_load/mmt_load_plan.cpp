#include <pch.h>
#include <op/mmt_load/mmt_load_plan.h>
#include <op/mmt_load/mmt_load_action.h>

namespace msdb
{
namespace core
{
mmt_load_plan::mmt_load_plan()
{
}
const char* mmt_load_plan::name()
{
	return "mmt_load_plan";
}

pAction mmt_load_plan::makeAction()
{
	return std::make_shared<mmt_load_action>();
}
mmt_load_array_pset::mmt_load_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb