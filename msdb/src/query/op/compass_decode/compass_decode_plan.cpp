#include <pch.h>
#include <op/compass_decode/compass_decode_plan.h>
#include <op/compass_decode/compass_decode_action.h>

namespace msdb
{
namespace core
{
compass_decode_plan::compass_decode_plan()
{
}
const char* compass_decode_plan::name()
{
	return "compass_decode_plan";
}
pAction compass_decode_plan::makeAction()
{
	return std::make_shared<compass_decode_action>();
}

//////////////////////////////
// pset
compass_decode_array_pset::compass_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::CONST_TYPE);		// Target numBins
}
}		// core
}		// msdb