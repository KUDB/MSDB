#include <pch.h>
#include <api_cpp/cpp_operators.h>
#include <op/load/load_plan.h>
#include <op/between/between_plan.h>
#include <op/copy_to_buffer/copy_to_buffer_plan.h>
#include <array/dimensionDesc.h>
#include <array/attributeDesc.h>

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

/* ************************ */
/* Build					*/
/* ************************ */
BuildOpr::BuildOpr(std::shared_ptr<AFLOperator> qry, Domain d)
	: childQry_(qry), domain_(d), AFLOperator(qry->getArrayDesc())
{
}

BuildOpr& AddAxis(uint32_t dimId, std::string name, Coordinate dim, int64_t chunkSize, int64_t blockSize)
{

}

std::shared_ptr<BuildOpr> Build(std::shared_ptr<AFLOperator> qry, Domain d)
{
	return std::make_shared<BuildOpr>(qry, d);
}

/* ************************ */
/* Filter					*/
/* ************************ */
FilterOpr::FilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred)
	: childQry_(qry), pred_(pred), AFLOperator(qry->getArrayDesc())
{
}
std::shared_ptr<core::opPlan> FilterOpr::getPlan()
{
	return std::shared_ptr<core::opPlan>();
}
std::shared_ptr<FilterOpr> Filter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm)
{
	return std::make_shared<FilterOpr>(qry, std::make_shared<PredicateImpl>(std::make_shared<core::singlePredicate>(singleTerm->getTerm())));
}

/* ************************ */
/* ToBuffer					*/
/* ************************ */
CopyToBufferOpr::CopyToBufferOpr(std::shared_ptr<AFLOperator> qry)
	: childQry_(qry), AFLOperator(qry->getArrayDesc())
{
}
std::shared_ptr<core::opPlan> CopyToBufferOpr::getPlan()
{
	auto qryPlan = std::make_shared<core::copy_to_buffer_plan>();

	core::parameters params = {
		std::make_shared<core::opParamPlan>(childQry_->getPlan())
	};
	qryPlan->setParamSet(std::make_shared<core::copy_to_buffer_pset>(params));
	
	return qryPlan;
}
std::shared_ptr<CopyToBufferOpr> CopyToBuffer(std::shared_ptr<AFLOperator> qry)
{
	return std::shared_ptr<CopyToBufferOpr>();
}
}		// msdb