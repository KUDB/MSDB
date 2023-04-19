#pragma once
#ifndef _MSDB_COMPASS_INDEX_BUILD_ACTION_H_
#define _MSDB_COMPASS_INDEX_BUILD_ACTION_H_

#include <query/opAction.h>
#include <query/userDefinedOp.h>
#include <query/operatorLibrary.h>

namespace msdb
{
namespace core
{
class compass_index_build_action : public opAction
{
public:
	compass_index_build_action();
	virtual ~compass_index_build_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
};
}		// core
}		// msdb
#endif	// _MSDB_COMPASS_INDEX_BUILD_ACTION_H_