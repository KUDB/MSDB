#pragma once
#ifndef _MSDB_OP_PROJECT_PLAN_H_
#define _MSDB_OP_PROJECT_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/userDefinedOp.h>
#include <query/operatorLibrary.h>

namespace msdb
{
namespace op
{
using namespace msdb::core;

class OP_DLL_API project_plan : public opPlan
{
public:
	project_plan();
	~project_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class OP_DLL_API project_pset : public opPlanParamSet
{
public:

public:
	project_pset(parameters& pSet);
};

REGISTER_USER_DEFINED_OPERATOR_FACTORY(project, project_plan);
}		// op
}		// msdb
#endif	// _MSDB_OP_PROJECT_PLAN_H_