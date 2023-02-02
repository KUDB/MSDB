#pragma once
#ifndef _MSDB_OP_LZW_HUFFMAN_DECODE_PLAN_H_
#define _MSDB_OP_LZW_HUFFMAN_DECODE_PLAN_H_

#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <query/userDefinedOp.h>
#include <query/operatorLibrary.h>

namespace msdb
{
namespace core
{
class OP_DLL_API lzw_huffman_decode_plan : public opPlan
{
public:
	lzw_huffman_decode_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class OP_DLL_API lzw_huffman_decode_array_pset : public opArrayParamSet
{
public:
	lzw_huffman_decode_array_pset(parameters& pSet);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LZW_HUFFMAN_DECODE_PLAN_H_