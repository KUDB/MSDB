#include <pch.h>
#include <op/lzw_encode/lzw_encode_plan.h>
#include <op/lzw_encode/lzw_encode_action.h>

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
}		// core
}		// msdb