#pragma once
#ifndef _MSDB_OP_COMPASS_DECODE_PLAN_H_
#define _MSDB_OP_COMPASS_DECODE_PLAN_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class OP_DLL_API compass_decode_plan : public opPlan
{
public:
	compass_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// ParamSets
class OP_DLL_API compass_decode_array_pset : public opArrayParamSet
{
public:
	compass_decode_array_pset(parameters& pSet);
};
class OP_DLL_API compass_decode_plan_pset : public opPlanParamSet
{
public:
	compass_decode_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif // _MSDB_OP_COMPASS_DECODE_PLAN_H_