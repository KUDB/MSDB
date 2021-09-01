#include <pch.h>
#include <op/lzw_decode/lzw_decode_plan.h>
#include <op/lzw_decode/lzw_decode_action.h>

namespace msdb
{
namespace core
{
lzw_decode_plan::lzw_decode_plan()
{
}
const char* lzw_decode_plan::name()
{
	return "lzw_decode_plan";
}
pAction lzw_decode_plan::makeAction()
{
	return std::make_shared<lzw_decode_action>();
}
lzw_decode_array_pset::lzw_decode_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{

}
}		// core
}		// msdb