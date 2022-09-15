#pragma once
#ifndef _MSDB_EXAMPLE_EXP_FILTER_H_
#define _MSDB_EXAMPLE_EXP_FILTER_H_

#include <api_cpp/msdb.h>

namespace msdb
{
namespace experiments
{
namespace filter
{
static const int filterExpId = 5000;
static const int filterValueSeed = 2021;
static const int valueLimit = 256;
static const int numTest = 3;
}		// filter

std::shared_ptr<FilterOpr> makeFilterEqQry(
	std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value);
std::shared_ptr<IndexFilterOpr> makeIndexFilterEqQry(
	std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value);

std::shared_ptr<FilterOpr> makeFilterLessQry(
	std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value);

std::vector<core::pTimer> exeFilterTest(
	const int numTest, std::shared_ptr<AFLOperator> childQry, std::string attrName);

namespace data_star1024x1024
{
std::vector<core::pTimer> exeFilterTest(const int numTest, encodingType compType);
}
namespace data_solar1024x1024
{
std::vector<core::pTimer> exeFilterTest(const int numTest, encodingType compType);
}
namespace data_saturn1024x1024
{
std::vector<core::pTimer> exeFilterTest(const int numTest, encodingType compType);
}
namespace data_mars4096x2048
{
std::vector<core::pTimer> exeFilterTest(const int numTest, encodingType compType);
}
namespace data_mercury20480x10240
{
std::vector<core::pTimer> exeFilterTest(const int numTest, encodingType compType);
}
namespace data_lunar102400x40960
{
std::vector<core::pTimer> exeFilterTest(const int numTest, encodingType compType);
}
}		// experiments
}		// msdb
#endif	// _MSDB_EXAMPLE_EXP_FILTER_H_