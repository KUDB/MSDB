#pragma once
#ifndef _MSDB_OP_ZIP_ENCODE_PLAN_H_
#define _MSDB_OP_ZIP_ENCODE_PLAN_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class OP_API zip_save_plan : public opPlan
{
public:
	zip_save_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// ParamSets
class OP_API zip_save_array_pset : public opArrayParamSet
{
public:
	zip_save_array_pset(parameters& pSet);
};
class OP_API zip_save_plan_pset : public opPlanParamSet
{
public:
	zip_save_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZIP_ENCODE_PLAN_H_