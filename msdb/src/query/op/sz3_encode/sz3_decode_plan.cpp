#include <pch.h>
#include <op/sz3_encode/sz3_decode_plan.h>
#include <op/sz3_encode/sz3_decode_action.h>

namespace msdb
{
namespace core
{
sz3_decode_plan::sz3_decode_plan()
{
}
const char* sz3_decode_plan::name()
{
	return "sz3_decode_plan";
}
pAction sz3_decode_plan::makeAction()
{
	return std::make_shared<sz3_decode_action>();
}

//////////////////////////////
// pset
sz3_decode_array_pset::sz3_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb