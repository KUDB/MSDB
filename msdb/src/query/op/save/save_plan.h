#pragma once
#ifndef _MSDB_OP_SAVE_PLAN_H_
#define _MSDB_OP_SAVE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class save_plan : public opPlan
{
public:
	save_plan();
	~save_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class save_array_pset : public opArrayParamSet
{
public:
	save_array_pset(parameters& pSet);
};

class save_plan_pset : public opPlanParamSet
{
public:
	save_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SAVE_PLAN_H_