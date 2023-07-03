#pragma once

#ifndef _MSDB_DUMMY_QUERY_UTIL_H_
#define _MSDB_DUMMY_QUERY_UTIL_H_

#include <api_cpp/msdb.h>
#include <iostream>

namespace msdb
{
namespace dummy
{
msdb::Query exeQuery(std::shared_ptr<AFLOperator> afl, bool printAFL = true, bool printResult = false, bool printTimer = false);
msdb::Query exeQuery_print_Timer(std::shared_ptr<AFLOperator> afl);
msdb::Query exeQuery_print_Result_Timer(std::shared_ptr<AFLOperator> afl);

////////////////////////////////////////
// Range
////////////////////////////////////////
msdb::Query exeRangeQry(const std::string& arrName, const core::encodingType& compType, const msdb::Domain& range);
msdb::Query exeRangeQryPrintResult(const std::string& arrName, const core::encodingType& compType, const msdb::Domain& range);

////////////////////////////////////////
// Filter
////////////////////////////////////////
msdb::Query exeFilterQry(const std::string& arrName, const core::encodingType& compType, const int64_t value);
msdb::Query exeFilterQryPrintResult(const std::string& arrName, const core::encodingType& compType, const int64_t value);
msdb::Query exeIndexFilterQry(const std::string& arrName, const core::encodingType& compType, const int64_t value);
msdb::Query exeIndexFilterQryPrintResult(const std::string& arrName, const core::encodingType& compType, const int64_t value);

////////////////////////////////////////
// Range-Filter
////////////////////////////////////////
msdb::Query exeRangeFilterQry(const std::string& arrName, const core::encodingType& compType,
							  const msdb::Domain& range, const int64_t value);
msdb::Query exeRangeFilterQryPrintResult(const std::string& arrName, const core::encodingType& compType,
										 const msdb::Domain& range, const int64_t value);
msdb::Query exeRangeIndexFilterQry(const std::string& arrName, const core::encodingType& compType,
								   const msdb::Domain& range, const int64_t value);
msdb::Query exeRangeIndexFilterQryPrintResult(const std::string& arrName, const core::encodingType& compType,
											  const msdb::Domain& range, const int64_t value);

////////////////////////////////////////
// Verify
////////////////////////////////////////
bool verify(const std::string& arrName, msdb::Query& qry);
}
}
#endif	// _MSDB_DUMMY_QUERY_UTIL_H_