#pragma once
#include <dummy_array.h>
#include <query/query.h>

#ifndef _MSDB_DUMMY_H_
#define _MSDB_DUMMY_H_

namespace msdb
{
namespace dummy
{
void tearDownQuery(core::pTimer myTimer, size_t expId,
				   size_t expTrial, size_t dataId, size_t methodId);
}		// dummy
}		// msdb

#endif	// _MSDB_DUMMY_H_