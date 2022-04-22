#include <pch.h>
#include <op/zfp_encode/zfp_decode_plan.h>
#include <op/zfp_encode/zfp_decode_action.h>

namespace msdb
{
namespace core
{
zfp_decode_plan::zfp_decode_plan()
{
}
const char* zfp_decode_plan::name()
{
	return "zfp_decode_plan";
}
pAction zfp_decode_plan::makeAction()
{
	return std::make_shared<zfp_decode_action>();
}

//////////////////////////////
// pset
zfp_decode_array_pset::zfp_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb