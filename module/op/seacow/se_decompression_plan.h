#pragma once
#ifndef _MSDB_SE_DECOMPRESSION_PLAN_H_
#define _MSDB_SE_DECOMPRESSION_PLAN_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/userDefinedOp.h>

namespace msdb
{
namespace core
{
class OP_API se_decompression_plan : public opPlan
{
public:
	se_decompression_plan();
	~se_decompression_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class OP_API se_decompression_array_pset : public opArrayParamSet
{
public:
	se_decompression_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}		// core
}		// msdb
#endif	// _MSDB_SE_DECOMPRESSION_PLAN_H_