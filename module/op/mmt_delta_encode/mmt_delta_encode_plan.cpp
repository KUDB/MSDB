#include <pch.h>
#include <op/mmt_delta_encode/mmt_delta_encode_plan.h>
#include <op/mmt_delta_encode/mmt_delta_encode_action.h>

namespace msdb
{
namespace core
{
mmt_delta_encode_plan::mmt_delta_encode_plan()
{
}

mmt_delta_encode_plan::~mmt_delta_encode_plan()
{
}

const char* mmt_delta_encode_plan::name()
{
	return "mmt_delta_encode_plan";
}

pAction mmt_delta_encode_plan::makeAction()
{
	return std::make_shared<mmt_delta_encode_action>();
}

mmt_delta_encode_array_pset::mmt_delta_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb