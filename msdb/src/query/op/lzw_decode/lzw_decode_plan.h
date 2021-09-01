#pragma once
#ifndef _MSDB_OP_LZW_DECODE_PLAN_H_
#define _MSDB_OP_LZW_DECODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class lzw_decode_plan : public opPlan
{
public:
	lzw_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class lzw_decode_array_pset : public opArrayParamSet
{
public:
	lzw_decode_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LZW_DECODE_PLAN_H_