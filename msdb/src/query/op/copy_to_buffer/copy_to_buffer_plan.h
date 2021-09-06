#pragma once
#ifndef _MSDB_OP_COPY_TO_BUFFER_PLAN_H_
#define _MSDB_OP_COPY_TO_BUFFER_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class copy_to_buffer_plan : public opPlan
{
public:
	copy_to_buffer_plan();
	~copy_to_buffer_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class copy_to_buffer_pset : public opPlanParamSet
{
public:
	copy_to_buffer_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_COPY_TO_BUFFER_PLAN_H_