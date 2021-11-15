#pragma once
#ifndef _MSDB_EXAMPLE_EXP_RANGE_H_
#define _MSDB_EXAMPLE_EXP_RANGE_H_

#include <api_cpp/msdb.h>

namespace msdb
{
namespace experiments
{
namespace rangeFilter
{
static const int filterExpId = 7000;
static const int filterValueSeed = 2021;
static const int valueLimit = 256;
static const int numTest = 3;
static const std::vector<float> selectivity({ 0.01, 0.1, 1, 10 });
}		// rangeFilter

Domain getRandomRange(const uint64_t dimX, const uint64_t dimY, const float selectivity);

std::shared_ptr<BetweenOpr> makeRangeFilterEqQry(
	std::shared_ptr<AFLOperator> opt, std::string attrName, Domain d, int64_t value);
std::shared_ptr<BetweenOpr> makeRangeIndexFilterEqQry(
	std::shared_ptr<AFLOperator> opt, std::string attrName, Domain d, int64_t value);

std::vector<core::pTimer> exeRangeFilterTest(
	const int numTest, std::shared_ptr<AFLOperator> childQry, 
	Coordinate arraySize, std::string attrName);

namespace data_star1024x1024
{
std::vector<core::pTimer> exeRangeTest(const int numTest, compressionType compType);
}
namespace data_solar1024x1024
{
std::vector<core::pTimer> exeRangeTest(const int numTest, compressionType compType);
}
namespace data_saturn1024x1024
{
std::vector<core::pTimer> exeRangeTest(const int numTest, compressionType compType);
}
namespace data_mars4096x2048
{
std::vector<core::pTimer> exeRangeTest(const int numTest, compressionType compType);
}
namespace data_mercury20480x10240
{
std::vector<core::pTimer> exeRangeTest(const int numTest, compressionType compType);
}
namespace data_lunar102400x40960
{
std::vector<core::pTimer> exeRangeTest(const int numTest, compressionType compType);
}
}		// experiments
}		// msdb
#endif	// _MSDB_EXAMPLE_EXP_RANGE_H_