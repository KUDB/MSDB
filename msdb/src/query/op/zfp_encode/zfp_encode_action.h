#pragma once
#ifndef _MSDB_OP_ZFP_ENCODE_ACTION_H_
#define _MSDB_OP_ZFP_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class zfp_encode_action : public opIOAction
{
public:
	zfp_encode_action();
	virtual ~zfp_encode_action();
	virtual inline const char* name() override
	{
		return "zfp_encode";
	}
	virtual inline int encodingType() override
	{
		return encodingType::ZFP;
	}

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZFP_ENCODE_ACTION_H_