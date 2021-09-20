#pragma once
#ifndef _MSDB_OP_SE_COMPRESSION_PLAN_H_
#define _MSDB_OP_SE_COMPRESSION_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class se_compression_plan : public opPlan
{
public:
	se_compression_plan();
	~se_compression_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class se_compression_array_pset : public opArrayParamSet
{
public:
	se_compression_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SE_COMPRESSION_PLAN_H_