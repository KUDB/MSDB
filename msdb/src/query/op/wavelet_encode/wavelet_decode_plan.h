#pragma once
#ifndef _MSDB_OP_WAVELET_DECODE_PLAN_H_
#define _MSDB_OP_WAVELET_DECODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class wavelet_decode_plan : public opPlan
{
public:
	wavelet_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class wavelet_decode_array_pset : public opArrayParamSet
{
public:
	wavelet_decode_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
};

class wavelet_decode_plan_pset : public opPlanParamSet
{
public:
	wavelet_decode_plan_pset(parameters& pSet);

public:
	//virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;
	virtual pBitmapTree inferTopDownBitmap(pBitmapTree fromParent) override;
};
}		// core
}		// msdb
#endif		// _MSDB_OP_WAVELET_DECODE_PLAN_H_