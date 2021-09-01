#include <pch.h>
#include <op/zip_load/zip_load_plan.h>
#include <op/zip_load/zip_load_action.h>

namespace msdb
{
namespace core
{
zip_load_plan::zip_load_plan()
{
}
const char* zip_load_plan::name()
{
	return "zip_load_plan";
}
pAction zip_load_plan::makeAction()
{
	return std::make_shared<zip_load_action>();
}

//////////////////////////////
// pset
zip_load_array_pset::zip_load_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb