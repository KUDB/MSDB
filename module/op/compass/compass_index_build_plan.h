#pragma once
#ifndef _MSDB_COMPASS_INDEX_BUILD_PLAN_H_
#define _MSDB_COMPASS_INDEX_BUILD_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/userDefinedOp.h>
#include <query/operatorLibrary.h>

namespace msdb
{
namespace core
{
class OP_DLL_API compass_index_build_plan : public opPlan
{
public:
	compass_index_build_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// ParamSets
class OP_DLL_API compass_index_build_array_pset : public opArrayParamSet
{
public:
	compass_index_build_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
class OP_DLL_API compass_index_build_plan_pset : public opPlanParamSet
{
public:
	compass_index_build_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif		// _MSDB_COMPASS_INDEX_BUILD_PLAN_H_