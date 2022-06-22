#pragma once
#ifndef _MSDB_OP_ZFP_ENCODE_ACTION_H_
#define _MSDB_OP_ZFP_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class zfp_encode_action : public opAction
{
public:
	using base_type = opAction;

public:
	zfp_encode_action();
	virtual ~zfp_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZFP_ENCODE_ACTION_H_