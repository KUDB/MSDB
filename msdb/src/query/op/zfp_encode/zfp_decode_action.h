#pragma once
#ifndef _MSDB_OP_ZFP_DECODE_ACTION_H_
#define _MSDB_OP_ZFP_DECODE_ACTION_H_

#include <pch.h>
#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class zfp_decode_action : public opIOAction
{
public:
	zfp_decode_action();
	virtual ~zfp_decode_action();
	virtual const char* name() override
	{
		return "zfp_decode";
	}
	virtual inline int encodingType() override
	{
		return encodingType::ZFP;
	}
	virtual pArray getOutArray(pArrayDesc desc) override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZFP_DECODE_ACTION_H_