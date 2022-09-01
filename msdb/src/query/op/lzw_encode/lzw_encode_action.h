#pragma once
#ifndef _MSDB_OP_LZW_ENCODE_ACTION_H_
#define _MSDB_OP_LZW_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class lzw_encode_action : public opIOAction
{
public:
	lzw_encode_action();
	virtual ~lzw_encode_action();
	virtual const char* name() override
	{
		return "lzw_encode";
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
#endif	// _MSDB_OP_LZW_ENCODE_ACTION_H_