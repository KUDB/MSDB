#pragma once
#ifndef _MSDB_OP_DECODE_RAW_PLAN_H_
#define _MSDB_OP_DECODE_RAW_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class decode_raw_plan : public opPlan
{
public:
	decode_raw_plan();
	~decode_raw_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class decode_raw_array_pset : public opArrayParamSet
{
public:
	decode_raw_array_pset(parameters& pSet);
};

//class decode_raw_plan_pset : public opPlanParamSet
//{
//public:
//	decode_raw_plan_pset(parameters& pSet);
//};
}		// core
}		// msdb
#endif		// _MSDB_OP_DECODE_RAW_PLAN_H_