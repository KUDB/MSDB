#pragma once
#ifndef _MSDB_OP_LZW_DECODE_ACTION_H_
#define _MSDB_OP_LZW_DECODE_ACTION_H_

#include <pch.h>
#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class lzw_decode_action : public opIOAction
{
public:
	lzw_decode_action();
	virtual ~lzw_decode_action();
	virtual const char* name() override
	{
		return "lzw_decode";
	}
	virtual inline int encodingType() override
	{
		return encodingType::LZW;
	}
	virtual pArray getOutArray(pArrayDesc desc) override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif // _MSDB_OP_LZW_DECODE_ACTION_H_