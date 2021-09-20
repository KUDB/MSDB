#pragma once
#ifndef _MSDB_OP_MMT_LOAD_PLAN_H_
#define _MSDB_OP_MMT_LOAD_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class mmt_load_plan : public opPlan
{
public:
	mmt_load_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class mmt_load_array_pset : public opArrayParamSet
{
public:
	mmt_load_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_MMT_LOAD_PLAN_H_