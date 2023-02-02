#include <pch_op.h>
#include <op/copy_to_buffer/copy_to_buffer_plan.h>
#include <op/copy_to_buffer/copy_to_buffer_action.h>

namespace msdb
{
namespace core
{
copy_to_buffer_plan::copy_to_buffer_plan()
{

}
copy_to_buffer_plan::~copy_to_buffer_plan()
{

}
const char* copy_to_buffer_plan::name()
{
	return "copy_to_buffer";
}
pAction copy_to_buffer_plan::makeAction()
{
	return std::make_shared<copy_to_buffer_action>();
}

copy_to_buffer_pset::copy_to_buffer_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
}		// core
}		// msdb