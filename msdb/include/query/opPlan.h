#pragma once
#ifndef _MSDB_OPPLAN_H_
#define _MSDB_OPPLAN_H_

#include <pch.h>
#include <query/opParamSet.h>
#include <query/opAction.h>
#include <util/status.h>

namespace msdb
{
namespace core
{
class query;
class opPlan;
using pPlan = std::shared_ptr<opPlan>;

class opPlan : public std::enable_shared_from_this<opPlan>
{
public:
	opPlan();

public:
	virtual const char* name() = 0;
	void setParamSet(pParamSet paramSet);
	virtual pArrayDesc inferSchema();
	virtual pBitmapTree inferInBitmap();
	virtual pAction getAction();
	parameters getParam();
	virtual pArray process(std::shared_ptr<query> qry);
	
protected:
	virtual pBitmapTree inferBottomUpBitmap();
	friend pBitmapTree opPlanParamSet::getSourcePlanBottomUpBitmap();

	virtual pBitmapTree inferTopDownBitmap();
	virtual pAction makeAction() = 0;

protected:
	virtual void setParentPlan(pPlan parentPlan);

private:
	pParamSet inParamSet_;
	pBitmapTree inArrBitmap_;
	pBitmapTree outArrBitmap_;
	pPlan parentPlan_;
	std::vector<pPlan> childPlans_;
};
}		// core
}		// msdb
#endif	// _MSDB_OPPLAN_H_
