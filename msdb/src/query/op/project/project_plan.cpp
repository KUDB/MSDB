#include <pch.h>
#include "project_plan.h"
#include "project_action.h"

namespace msdb
{
namespace core
{
project_plan::project_plan()
{
}
project_plan::~project_plan()
{
}
const char* project_plan::name()
{
	return "project";
}

pAction project_plan::makeAction()
{
	return std::make_shared<project_action>();
}

project_pset::project_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 2);

	assert(this->params_[1]->type() == opParamType::STRING_LIST);
}
}		// core
}		// msdb