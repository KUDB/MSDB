#pragma once
#ifndef _MSDB_EXAMPLE_EXP_FILTER_H_
#define _MSDB_EXAMPLE_EXP_FILTER_H_

#include <api_cpp/msdb.h>

namespace msdb
{
namespace experiments
{
static const int filterExpId = 5000;
static const int filterValueSeed = 21020;
static const int numTest = 3;

std::shared_ptr<FilterOpr> makeFilterEqQry(
	std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value);

std::shared_ptr<FilterOpr> makeFilterLessQry(
	std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value);

std::vector<core::pTimer> exeFilterTest(
	const int numTest, std::shared_ptr<AFLOperator> childQry, std::string attrName);

namespace data_star1024x1024
{
std::vector<core::pTimer> exeFilterTest(const int numTest, compressionType compType);
}
}		// experiments
}		// msdb
#endif	// _MSDB_EXAMPLE_EXP_FILTER_H_