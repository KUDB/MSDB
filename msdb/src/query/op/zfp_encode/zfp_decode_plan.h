#pragma once
#ifndef _MSDB_OP_ZFP_DECODE_PLAN_H_
#define _MSDB_OP_ZFP_DECODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class zfp_decode_plan : public opPlan
{
public:
	zfp_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class zfp_decode_array_pset : public opArrayParamSet
{
public:
	zfp_decode_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZFP_DECODE_PLAN_H_