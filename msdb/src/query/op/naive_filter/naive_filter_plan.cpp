#include <pch.h>
#include <op/naive_filter/naive_filter_plan.h>
#include <op/naive_filter/naive_filter_action.h>
#include <cassert>

namespace msdb
{
namespace core
{
// plan
naive_filter_plan::naive_filter_plan()
{
}

const char* naive_filter_plan::name()
{
	return "naive_filter";
}

pAction naive_filter_plan::makeAction()
{
	return std::make_shared<naive_filter_action>();
}

// pset
naive_filter_array_pset::naive_filter_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::PREDICATE);
}

naive_filter_plan_pset::naive_filter_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[1]->type() == opParamType::PREDICATE);
}
}		// core
}		// msdb