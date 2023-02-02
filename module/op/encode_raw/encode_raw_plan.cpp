#include <pch_op.h>
#include <op/encode_raw/encode_raw_plan.h>
#include <op/encode_raw/encode_raw_action.h>

namespace msdb
{
namespace core
{
encode_raw_plan::encode_raw_plan()
{
}

encode_raw_plan::~encode_raw_plan()
{
}

const char* encode_raw_plan::name()
{
	return "encode_raw_plan";
}

pAction encode_raw_plan::makeAction()
{
	return std::make_shared<encode_raw_action>();
}

encode_raw_array_pset::encode_raw_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
encode_raw_plan_pset::encode_raw_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb