#include <pch.h>
#include <op/build/build_plan.h>
#include <array/arrayMgr.h>

namespace msdb
{
namespace core
{
build_plan::build_plan()
{
}
build_plan::~build_plan()
{
}
const char* build_plan::name()
{
	return "build";
}
pAction build_plan::makeAction()
{
	return std::make_shared<build_action>();
}
build_pset::build_pset(parameters& pSet)
	: opArrayParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
build_action::build_action()
{
}

build_action::~build_action()
{
}

const char* build_action::name()
{
	return "build";
}

pArray build_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{

	//arrayMgr::instance()->registArray();
	return nullptr;
}
}
}