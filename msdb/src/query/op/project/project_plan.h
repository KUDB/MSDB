#pragma once
#ifndef _MSDB_OP_PROJECT_PLAN_H_
#define _MSDB_OP_PROJECT_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class project_plan : public opPlan
{
public:
	project_plan();
	~project_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class project_pset : public opPlanParamSet
{
public:

public:
	project_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_PROJECT_PLAN_H_