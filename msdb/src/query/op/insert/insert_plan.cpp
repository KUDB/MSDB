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
	return "insert";
}

pAction insert_plan::makeAction()
{
	return std::make_shared<insert_action>();
}

insert_array_file_pset::insert_array_file_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 3);
	assert(this->params_[1]->type() == opParamType::ENUM);
	assert(this->params_[2]->type() == opParamType::STRING);
}
insert_array_memory_pset::insert_array_memory_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 4);
	assert(this->params_[1]->type() == opParamType::ENUM);
	assert(this->params_[2]->type() == opParamType::MEMORY);			// memory pointer
	assert(this->params_[3]->type() == opParamType::CONST_TYPE);		// memory size
}
}		// core
}		// msdb