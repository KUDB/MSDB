#include <pch.h>
#include <query/opPlan.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
opPlan::opPlan()
	: parentPlan_(nullptr), inArrBitmap_(nullptr), outArrBitmap_(nullptr), inParamSet_(nullptr)
{
}
void opPlan::setParamSet(pParamSet paramSet)
{
	this->inParamSet_ = paramSet;

	auto params = this->getParam();
	for (auto param : params)
	{
		if (param->type() == opParamType::PLAN)
		{
			auto childPlan = std::static_pointer_cast<opParamPlan::paramType>(param->getParam());
			childPlan->setParentPlan(shared_from_this());
		}
	}
}
pArrayDesc opPlan::inferSchema()
{
	return this->inParamSet_->inferSchema();
}
pBitmapTree opPlan::inferInBitmap()
{
	if (this->inArrBitmap_)
		return this->inArrBitmap_;

	this->outArrBitmap_ = this->inferBottomUpBitmap();
	this->inArrBitmap_ = this->inferTopDownBitmap();

	return inArrBitmap_;
}
pBitmapTree opPlan::inferBottomUpBitmap()
{
	return this->inParamSet_->inferBottomUpBitmap();
}
pBitmapTree opPlan::inferTopDownBitmap()
{
	if (this->parentPlan_)
	{
		this->outArrBitmap_ = this->parentPlan_->inferInBitmap();
	}

	assert(this->outArrBitmap_ != nullptr);
	return this->inParamSet_->inferTopDownBitmap(this->outArrBitmap_);
}
pAction opPlan::getAction()
{
	auto myAction = this->makeAction();
	auto arrDesc = this->inferSchema();
	myAction->setParams(this->getParam());
	myAction->setArrayDesc(arrDesc);
	this->inferInBitmap();
	myAction->setPlanInBitmap(this->inArrBitmap_);
	myAction->setPlanOutBitmap(this->outArrBitmap_);

	BOOST_LOG_TRIVIAL(debug) << "getAction: " << this->name() << " / " << arrDesc->name_ << "(" << arrDesc->id_ << ")";
	return myAction;
}
parameters opPlan::getParam()
{
	return this->inParamSet_->getParam();
}
void opPlan::setParentPlan(pPlan parentPlan)
{
	this->parentPlan_ = parentPlan;
}
}		// core
}		// msdb