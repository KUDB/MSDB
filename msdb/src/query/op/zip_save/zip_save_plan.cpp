#include <pch.h>
#include <op/zip_save/zip_save_plan.h>
#include <op/zip_save/zip_save_action.h>

namespace msdb
{
namespace core
{
zip_save_plan::zip_save_plan()
{
}
const char* zip_save_plan::name()
{
	return "zip_save_plan";
}
pAction zip_save_plan::makeAction()
{
	return std::make_shared<zip_save_action>();
}

//////////////////////////////
// pset
zip_save_array_pset::zip_save_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
	assert(this->params_[0]->type() == opParamType::ARRAY);			// Source array
}
}		// core
}		// msdb