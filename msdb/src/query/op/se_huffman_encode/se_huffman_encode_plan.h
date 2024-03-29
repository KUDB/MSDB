﻿#pragma once
#ifndef _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_
#define _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class se_huffman_encode_plan : public opPlan
{
public:
	se_huffman_encode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

//////////////////////////////
// ParamSets
class se_huffman_encode_array_pset : public opArrayParamSet
{
public:
	se_huffman_encode_array_pset(parameters& pSet);
};

class se_huffman_encode_plan_pset : public opPlanParamSet
{
public:
	se_huffman_encode_plan_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SE_HUFFMAN_ENCODE_PLAN_H_