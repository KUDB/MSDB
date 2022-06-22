#include <pch.h>
#include <op/consume/consume_plan.h>
#include <array/arrayMgr.h>

namespace msdb
{
namespace core
{
consume_plan::consume_plan()
{
}
consume_plan::~consume_plan()
{
}
const char* consume_plan::name()
{
	return "consume";
}
pAction consume_plan::makeAction()
{
	return std::make_shared<consume_action>();
}
consume_plan_pset::consume_plan_pset(parameters& pSet)
	: opPlanParamSet(pSet)
{
	assert(this->params_.size() == 1);
}
consume_action::consume_action()
{
}

consume_action::~consume_action()
{
}

const char* consume_action::name()
{
	return "consume";
}

pArray consume_action::execute(std::vector<pArray>& inputArrays, pQuery qry)
{
	auto arr = inputArrays[0];
	return arr;
}
}
}