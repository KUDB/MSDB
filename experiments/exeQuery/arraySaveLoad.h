#pragma once
#ifndef _MSDB_EXPERIMENTS_ARRAYSAVELOAD_H_
#define _MSDB_EXPERIMENTS_ARRAYSAVELOAD_H_

#include <pch_experiments.h>

namespace msdb
{
namespace dummy
{
msdb::Query executeInsertSaveArray(const std::string& arrName, const std::string& filePath,
								   const core::compressionType& compType = core::compressionType::NONE);

msdb::Query executeLoadArray(const std::string& arrName, const core::compressionType& compType);

msdb::Query executeRangeQryArray(const std::string& arrName, const core::compressionType& compType, const msdb::Domain& range);
}
}

#endif	// _MSDB_EXPERIMENTS_ARRAYSAVELOAD_H_