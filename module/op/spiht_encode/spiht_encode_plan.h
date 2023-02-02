#pragma once
#ifndef _MSDB_OP_SPIHT_ENCODE_PLAN_H_
#define _MSDB_OP_SPIHT_ENCODE_PLAN_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class OP_API spiht_encode_plan : public opPlan
{
public:
	spiht_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// ParamSets
class OP_API spiht_encode_array_pset : public opArrayParamSet
{
public:
	spiht_encode_array_pset(parameters& pSet);
};
class OP_API spiht_encode_plan_pset : public opPlanParamSet
{
public:
	spiht_encode_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif // _MSDB_OP_SPIHT_ENCODE_PLAN_H_