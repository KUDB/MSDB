#pragma once
#ifndef _MSDB_MMT_BUILD_PLAN_H_
#define _MSDB_MMT_BUILD_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class mmt_build_plan : public opPlan
{
public:
	mmt_build_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class mmt_build_array_pset : public opArrayParamSet
{
public:
	mmt_build_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}		// core
}		// msdb
#endif	// _MSDB_MMT_BUILD_PLAN_H_