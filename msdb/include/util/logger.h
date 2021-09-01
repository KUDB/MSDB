#pragma once
#ifndef _MSDB_LOGGER_H_
#define _MSDB_LOGGER_H_

#include <pch.h>

namespace msdb
{
namespace core
{
//////////////////////////////
// Trivial logging with filters
//////////////////////////////
//
// - trace
// - debug
// - info
// - warning
// - error
// - fatal
//

bool initBoostLogger();
}		// core
}		// msdb
#endif	// _MSDB_LOGGER_H_
 