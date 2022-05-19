#include <pch.h>
#include <op/tthresh_encode/tthresh_encode_plan.h>
#include <op/tthresh_encode/tthresh_encode_action.h>

namespace msdb
{
namespace core
{
tthresh_encode_plan::tthresh_encode_plan()
{
}
const char* tthresh_encode_plan::name()
{
	return "tthresh_encode_plan";
}
pAction tthresh_encode_plan::makeAction()
{
	return std::make_shared<tthresh_encode_action>();
}

//////////////////////////////
// pset
tthresh_encode_array_pset::tthresh_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
tthresh_encode_plan_pset::tthresh_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb