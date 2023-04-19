#include "lzw_encode_plan.h"
#include "lzw_encode_action.h"

namespace msdb
{
namespace core
{
lzw_encode_plan::lzw_encode_plan()
{
}
const char* lzw_encode_plan::name()
{
	return "lzw_encode_plan";
}
pAction lzw_encode_plan::makeAction()
{
	return std::make_shared<lzw_encode_action>();
}

//////////////////////////////
// pset
lzw_encode_array_pset::lzw_encode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}

lzw_encode_plan_pset::lzw_encode_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{

}
}		// core
}		// msdb