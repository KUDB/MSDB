#include <pch.h>
#include <op/mmt_delta_decode/mmt_delta_decode_plan.h>
#include <op/mmt_delta_decode/mmt_delta_decode_action.h>

namespace msdb
{
namespace core
{
mmt_delta_decode_plan::mmt_delta_decode_plan()
{
}

mmt_delta_decode_plan::~mmt_delta_decode_plan()
{
}

const char* mmt_delta_decode_plan::name()
{
	return "mmt_delta_decode";
}

pAction mmt_delta_decode_plan::makeAction()
{
	return std::make_shared<mmt_delta_decode_action>();
}

mmt_delta_decode_array_pset::mmt_delta_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}

mmt_delta_decode_plan_pset::mmt_delta_decode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb