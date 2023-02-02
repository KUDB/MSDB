#pragma once
#ifndef _MSDB_OP_BUILD_H_
#define _MSDB_OP_BUILD_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class OP_API build_plan : public opPlan
{
public:
	build_plan();
	~build_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class OP_API build_array_pset : public opArrayParamSet
{
public:
	build_array_pset(parameters& pSet);
};

//////////////////////////////
// ParamSets
class OP_API build_action : public opAction
{
public:
	build_action();
	~build_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery qry) override;
};
}		// core
}		// msdb
#endif	// _MSDB_OP_BUILD_H_