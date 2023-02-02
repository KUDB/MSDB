#pragma once
#ifndef _MSDB_OP_LZW_ENCODE_PLAN_H_
#define _MSDB_OP_LZW_ENCODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/userDefinedOp.h>
#include <query/operatorLibrary.h>

namespace msdb
{
namespace core
{
class OP_DLL_API lzw_encode_plan : public opPlan
{
public:
	lzw_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// ParamSets
class OP_DLL_API lzw_encode_array_pset : public opArrayParamSet
{
public:
	lzw_encode_array_pset(parameters& pSet);
};

class OP_DLL_API lzw_encode_plan_pset : public opPlanParamSet
{
public:
	lzw_encode_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LZW_ENCODE_PLAN_H_