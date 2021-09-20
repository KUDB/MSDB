#pragma once
#ifndef _MSDB_COMPASS_INDEX_BUILD_PLAN_H_
#define _MSDB_COMPASS_INDEX_BUILD_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class compass_index_build_plan : public opPlan
{
public:
	compass_index_build_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class compass_index_build_array_pset : public opArrayParamSet
{
public:
	compass_index_build_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};
}		// core
}		// msdb
#endif		// _MSDB_COMPASS_INDEX_BUILD_PLAN_H_