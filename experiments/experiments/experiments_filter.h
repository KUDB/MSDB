#pragma once
#ifndef _MSDB_EXPERIMENTS_FILTER_H_
#define _MSDB_EXPERIMENTS_FILTER_H_

#include <pch_experiments.h>
#include <dummy_query_util.h>

namespace msdb
{
namespace experiments
{
static const int filterValueSeed = 21020;
static const int numTests = 3;

template <typename value_type>
void experimentFilterQry(const std::string& arrName, const core::compressionType& compType)
{
	srand(filterValueSeed);
	value_type v = 0;

	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		dummy::exeFilterQry(arrName, compType, v);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
}
template <typename value_type>
void experimentIndexFilterQry(const std::string& arrName, const core::compressionType& compType)
{
	srand(filterValueSeed);
	value_type v = 0;

	for (size_t i = 0; i < numTests; ++i)
	{
		v = (value_type)rand();
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		dummy::exeIndexFilterQry(arrName, compType, v);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
	}
}
}		// experiments
}		// msdb
#endif	// _MSDB_EXPERIMENTS_FILTER_H_