#include <pch.h>
#include <api_cpp/cpp_operators.h>
#include <op/load/load_plan.h>
#include <op/between/between_plan.h>

namespace msdb
{
/* ************************ */
/* AFLOperator				*/
/* ************************ */
AFLOperator::AFLOperator(core::pArrayDesc arrDesc)
	: arrDesc_(arrDesc)
{
}

/* ************************ */
/* Load						*/
/* ************************ */
LoadOpr::LoadOpr(Array arr)
	: AFLOperator(arr.getDesc())
{
}

std::shared_ptr<core::opPlan> LoadOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::load_plan>();

	core::parameters params = {
		std::make_shared<core::opParamArray>(this->getArrayDesc())
	};
	qryPlan->setParamSet(std::make_shared<core::load_array_pset>(params));

	return qryPlan;
}
std::shared_ptr<LoadOpr> Load(Array arr)
{
	return std::make_shared<LoadOpr>(arr);
}

/* ************************ */
/* Between					*/
/* ************************ */
BetweenOpr::BetweenOpr(Array arr, Domain d)
	: domain_(d), childQry_(Load(arr)), AFLOperator(arr.getDesc())
{
}

BetweenOpr::BetweenOpr(std::shared_ptr<AFLOperator> qry, Domain d)
	: childQry_(qry), domain_(d), AFLOperator(qry->getArrayDesc())
{
}

std::shared_ptr<core::opPlan> BetweenOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::between_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan()),
		std::make_shared<core::opParamCoor>(std::make_shared<core::coor>(this->domain_.getStart())),
		std::make_shared<core::opParamCoor>(std::make_shared<core::coor>(this->domain_.getEnd()))
	};
	qryPlan->setParamSet(std::make_shared<core::between_plan_pset>(params));

	return qryPlan;
}
std::shared_ptr<BetweenOpr> Between(Array arr, Domain d)
{
	return std::make_shared<BetweenOpr>(arr, d);
}
std::shared_ptr<BetweenOpr> Between(std::shared_ptr<AFLOperator> qry, Domain d)
{
	return std::make_shared<BetweenOpr>(qry, d);
}
}		// msdb