#pragma once
#ifndef _MSDB_QUERY_H_
#define _MSDB_QUERY_H_

#include <pch.h>
#include <util/timer.h>

namespace msdb
{
namespace core
{
class query;
using pQuery = std::shared_ptr<query>;

class query : std::enable_shared_from_this<query>
{
public:
	query();

public:
	pTimer getTimer();

protected:
	pTimer timer_;
};
}		// core
}		// msdb
#endif