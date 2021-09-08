#pragma once
#ifndef _MSDB_OP_INSERT_PLAN_H_
#define _MSDB_OP_INSERT_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class insert_plan : public opPlan
{
public:
	insert_plan();
	~insert_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class insert_array_pset : public opArrayParamSet
{
public:
	insert_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif // _MSDB_OP_INSERT_PLAN_H_
