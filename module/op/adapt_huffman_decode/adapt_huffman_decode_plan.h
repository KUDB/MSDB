#pragma once
#ifndef _MSDB_ADAPT_HUFFMAN_DECODE_PLAN_H_
#define _MSDB_ADAPT_HUFFMAN_DECODE_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
class adapt_huffman_decode_plan : public opPlan
{
public:
	adapt_huffman_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class adapt_huffman_decode_array_pset : public opArrayParamSet
{
public:
	adapt_huffman_decode_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_ADAPT_HUFFMAN_DECODE_PLAN_H_