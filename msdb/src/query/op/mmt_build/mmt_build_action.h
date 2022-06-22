#pragma once
#ifndef _MSDB_MMT_BUILD_ACTION_H_
#define _MSDB_MMT_BUILD_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class mmt_build_action : public opAction
{
public:
	mmt_build_action();
	virtual ~mmt_build_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
};
}		// core
}		// msdb
#endif	// _MSDB_MMT_BUILD_ACTION_H_