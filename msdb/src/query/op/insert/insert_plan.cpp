#include <pch.h>
#include <op/insert/insert_plan.h>
#include <op/insert/insert_action.h>

namespace msdb
{
namespace core
{
insert_plan::insert_plan()
{
}

insert_plan::~insert_plan()
{
}

const char* insert_plan::name()
{
	return "insert_plan";
}

pAction insert_plan::makeAction()
{
	return std::make_shared<insert_action>();
}

insert_array_pset::insert_array_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 2);
	assert(this->params_[0]->type() == opParamType::ARRAY);
	assert(this->params_[1]->type() == opParamType::STRING);
}
}		// core
}		// msdb