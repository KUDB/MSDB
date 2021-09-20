#include <pch.h>
#include <op/mmt_save/mmt_save_plan.h>
#include <op/mmt_save/mmt_save_action.h>

namespace msdb
{
namespace core
{
mmt_save_plan::mmt_save_plan()
{
}

const char* mmt_save_plan::name()
{
	return "mmt_save_plan";
}
pAction mmt_save_plan::makeAction()
{
	return std::make_shared<mmt_save_action>();
}
mmt_save_array_pset::mmt_save_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb