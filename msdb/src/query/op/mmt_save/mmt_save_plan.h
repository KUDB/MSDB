#pragma once
#ifndef _MSDB_OP_MMT_SAVE_PLAN_H_
#define _MSDB_OP_MMT_SAVE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class mmt_save_plan : public opPlan
{
public:
	mmt_save_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class mmt_save_array_pset : public opArrayParamSet
{
public:
	mmt_save_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif		// _MSDB_OP_MMT_SAVE_PLAN_H_
