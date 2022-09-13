#pragma once
#ifndef _MSDB_OP_PROJECT_PLAN_H_
#define _MSDB_OP_PROJECT_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/userDefinedOp.h>
#include <query/operatorLibrary.h>
#include <api_cpp/cpp_operators.h>

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
	using containerType = std::vector<std::string>;
	using containerParamType = opParamStringList;

public:
	project_pset(parameters& pSet);
};

class projectOpr : public msdb::AFLOperator
{
public:
	projectOpr(Array arr, project_pset::containerType& params);
	projectOpr(std::shared_ptr<AFLOperator> qry, project_pset::containerType& params);

public:
	std::shared_ptr<core::opPlan> getPlan() override;
	std::string toString(int depth) override;

private:
	std::shared_ptr<AFLOperator> childQry_;
	std::shared_ptr<std::vector<std::string>> attrList_;
};

std::shared_ptr<projectOpr> OP_DLL_API Project(Array arr, project_pset::containerType& attrList);
std::shared_ptr<projectOpr> OP_DLL_API Project(std::shared_ptr<AFLOperator> qry, project_pset::containerType& attrList);
}		// op
}		// msdb
#endif	// _MSDB_OP_PROJECT_PLAN_H_