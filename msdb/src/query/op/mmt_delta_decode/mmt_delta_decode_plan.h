#pragma once
#ifndef _MSDB_OP_MMT_DELTA_DECODE_PLAN_H_
#define _MSDB_OP_MMT_DELTA_DECODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class mmt_delta_decode_plan : public opPlan
{
public:
	mmt_delta_decode_plan();
	~mmt_delta_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class mmt_delta_decode_array_pset : public opArrayParamSet
{
public:
	mmt_delta_decode_array_pset(parameters& pSet);
};

class mmt_delta_decode_plan_pset : public opPlanParamSet
{
public:
	mmt_delta_decode_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif		// _MSDB_OP_MMT_DELTA_DECODE_PLAN_H_