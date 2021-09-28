#pragma once
#ifndef _MSDB_OP_CONSUME_H_
#define _MSDB_OP_CONSUME_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class consume_plan : public opPlan
{
public:
	consume_plan();
	~consume_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class consume_plan_pset : public opPlanParamSet
{
public:
	consume_plan_pset(parameters& pSet);
};

class consume_action : public opAction
{
public:
	consume_action();
	~consume_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery qry) override;
};
}		// core
}		// msdb
#endif	// _MSDB_OP_CONSUME_H_