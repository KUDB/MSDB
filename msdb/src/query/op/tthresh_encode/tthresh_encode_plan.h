#pragma once
#ifndef _MSDB_OP_TTHRESH_ENCODE_PLAN_H_
#define _MSDB_OP_TTHRESH_ENCODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class tthresh_encode_plan : public opPlan
{
public:
	tthresh_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class tthresh_encode_array_pset : public opArrayParamSet
{
public:
	tthresh_encode_array_pset(parameters& pSet);
};
class tthresh_encode_plan_pset : public opPlanParamSet
{
public:
	tthresh_encode_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZFP_ENCODE_PLAN_H_