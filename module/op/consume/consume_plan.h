#pragma once
#ifndef _MSDB_OP_CONSUME_H_
#define _MSDB_OP_CONSUME_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class OP_API consume_plan : public opPlan
{
public:
	consume_plan();
	~consume_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class OP_API consume_plan_pset : public opPlanParamSet
{
public:
	consume_plan_pset(parameters& pSet);
};

class OP_API consume_action : public opAction
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