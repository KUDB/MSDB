#include <pch.h>
#include <op/tthresh_encode/tthresh_decode_plan.h>
#include <op/tthresh_encode/tthresh_decode_action.h>

namespace msdb
{
namespace core
{
tthresh_decode_plan::tthresh_decode_plan()
{
}
const char* tthresh_decode_plan::name()
{
	return "tthresh_decode_plan";
}
pAction tthresh_decode_plan::makeAction()
{
	return std::make_shared<tthresh_decode_action>();
}

//////////////////////////////
// pset
tthresh_decode_array_pset::tthresh_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb