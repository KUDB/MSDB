#pragma once
#ifndef _MSDB_OP_MMT_LOAD_ACTION_H_
#define _MSDB_OP_MMT_LOAD_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class mmt_load_action : public opAction
{
public:
	mmt_load_action();
	virtual ~mmt_load_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
};
}		// core
}		// msdb
#endif	// _MSDB_OP_MMT_LOAD_ACTION_H_