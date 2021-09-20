#pragma once
#ifndef _MSDB_OP_MMT_SAVE_ACTION_H_
#define _MSDB_OP_MMT_SAVE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class mmt_save_action : public opAction
{
public:
	mmt_save_action();
	virtual ~mmt_save_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif		// _MSDB_OP_MMT_SAVE_ACTION_H_