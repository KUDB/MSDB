#include <pch.h>
#include <op/zfp_encode/zfp_encode_plan.h>
#include <op/zfp_encode/zfp_encode_action.h>

namespace msdb
{
namespace core
{
zfp_encode_plan::zfp_encode_plan()
{
}
const char* zfp_encode_plan::name()
{
	return "zfp_encode_plan";
}
pAction zfp_encode_plan::makeAction()
{
	return std::make_shared<zfp_encode_action>();
}

//////////////////////////////
// pset
zfp_encode_array_pset::zfp_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
zfp_encode_plan_pset::zfp_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb