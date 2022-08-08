#pragma once
#ifndef _MSDB_OP_FAST_WAVELET_ENCODE_PLAN_H_
#define _MSDB_OP_FAST_WAVELET_ENCODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class fast_wavelet_encode_plan : public opPlan
{
public:
	fast_wavelet_encode_plan();

public:
	virtual const char* name() override;
	//virtual void initParamSets() override;
	virtual pAction makeAction() override;

};

class fast_wavelet_encode_array_pset : public opArrayParamSet
{
public:
	fast_wavelet_encode_array_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	// TODO::infer bitmap
};

class fast_wavelet_encode_plan_pset : public opPlanParamSet
{
public:
	fast_wavelet_encode_plan_pset(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;
};
}
}
#endif		// _MSDB_OP_FAST_WAVELET_ENCODE_PLAN_H_