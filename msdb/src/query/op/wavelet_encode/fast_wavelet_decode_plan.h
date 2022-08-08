#pragma once
#ifndef _MSDB_OP_FAST_WAVELET_DECODE_PLAN_H_
#define _MSDB_OP_FAST_WAVELET_DECODE_PLAN_H_

#include <pch.h>
#include "wavelet_decode_plan.h"

namespace msdb
{
namespace core
{
class fast_wavelet_decode_plan : public opPlan
{
public:
	fast_wavelet_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};
}		// core
}		// msdb
#endif		// _MSDB_OP_FAST_WAVELET_DECODE_PLAN_H_