#pragma once
#ifndef _MSDB_OP_COMPASS_ENCODE_ACTION_H_
#define _MSDB_OP_COMPASS_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <util/math.h>

namespace msdb
{
namespace core
{
class compass_encode_action : public opAction
{
public:
	compass_encode_action();
	virtual ~compass_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_COMPASS_ENCODE_ACTION_H_