#include <pch_op.h>
#include <op/encode_raw/decode_raw_plan.h>
#include <op/encode_raw/decode_raw_action.h>

namespace msdb
{
namespace core
{
decode_raw_plan::decode_raw_plan()
{
}

decode_raw_plan::~decode_raw_plan()
{
}

const char* decode_raw_plan::name()
{
	return "decode_raw_plan";
}

pAction decode_raw_plan::makeAction()
{
	return std::make_shared<decode_raw_action>();
}

decode_raw_array_pset::decode_raw_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
//decode_raw_plan_pset::decode_raw_plan_pset(parameters& pSet)
//	: opPlanParamSet(pSet)
//{
//	assert(this->params_.size() == 1);
//}
}		// core
}		// msdb