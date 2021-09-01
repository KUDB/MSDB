#include <pch.h>
#include <query/opParamSet.h>
#include <query/opPlan.h>

namespace msdb
{
namespace core
{
opParamSet::opParamSet()
{
	//this->initParams();
}
opParamSet::opParamSet(parameters& pSet)
{
	for(auto it = pSet.begin(); it != pSet.end(); it++)
	{
		this->params_.push_back((*it));
	}
}
parameters opParamSet::getParam()
{
	return this->params_;
}
opArrayParamSet::opArrayParamSet(parameters& pSet)
	: opParamSet(pSet)
{
	assert(this->params_[0]->type() == opParamType::ARRAY);		// source array
}
pArrayDesc opArrayParamSet::inferSchema()
{
	pArrayDesc aSourceDesc = std::static_pointer_cast<opParamArray::paramType>(this->params_[0]->getParam());
	return std::make_shared<opParamArray::paramType>(*aSourceDesc);
}
pBitmapTree opArrayParamSet::inferBottomUpBitmap()
{
	pArrayDesc desc = this->inferSchema();
	dimension chunkSpace = desc->getDimDescs()->getChunkSpace();

	return std::make_shared<bitmapTree>(chunkSpace.area(), true);
}
pBitmapTree opArrayParamSet::inferTopDownBitmap(pBitmapTree fromParent)
{
	return std::make_shared<bitmapTree>(*fromParent);
}

//////////////////////////////
// opPlanParam
opPlanParamSet::opPlanParamSet(parameters& pSet)
	: opParamSet(pSet)
{
	assert(this->params_[0]->type() == opParamType::PLAN);		// source plan
}
pArrayDesc opPlanParamSet::inferSchema()
{
	auto sourcePlan = std::static_pointer_cast<opParamPlan::paramType>(
		this->params_[0]->getParam());
	return std::make_shared<arrayDesc>(*sourcePlan->inferSchema());
}
pBitmapTree opPlanParamSet::inferBottomUpBitmap()
{
	return std::make_shared<bitmapTree>(*(this->getSourcePlanBottomUpBitmap()));
}
pBitmapTree opPlanParamSet::inferTopDownBitmap(pBitmapTree fromParent)
{
	return std::make_shared<bitmapTree>(*fromParent);
}
pBitmapTree opPlanParamSet::getSourcePlanBottomUpBitmap()
{
	auto sourcePlan = std::static_pointer_cast<opParamPlan::paramType>(
		this->params_[0]->getParam());
	return sourcePlan->inferBottomUpBitmap();
}
}		// core
}		// msdb