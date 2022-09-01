#pragma once
#ifndef _MSDB_EXPERIMENTS_FILTER_H_
#define _MSDB_EXPERIMENTS_FILTER_H_

#include <dummy.h>
#include <pch_experiments.h>
#include <dummy_query_util.h>

namespace msdb
{
namespace experiments
{
static const int filterValueSeed = 21020;
static const int numTests = 3;
static const int filterExpId = 10;

template <typename value_type>
void experimentFilterQry(const std::string& arrName, const core::encodingType& compType, 
						 const value_type min = std::numeric_limits<value_type>::min(), 
						 const value_type max = std::numeric_limits<value_type>::max())
{
	srand(filterValueSeed);
	value_type v = 0;

	for (size_t i = 1; i <= numTests; ++i)
	{
		v = (value_type)rand() % (max - min) + min;
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		auto q = dummy::exeFilterQry(arrName, compType, v);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		_MSDB_TRY_BEGIN
		{
			auto qry = q.getQueryObj();
			dummy::tearDownQuery(qry, filterExpId, i, qry->getArrayDesc()->id_, 0);
		}
			_MSDB_CATCH_ALL
		{
			BOOST_LOG_TRIVIAL(error) << "experimentFilterQry::Query recording error";
		}
	}
}
template <typename value_type>
void experimentIndexFilterQry(const std::string& arrName, const core::encodingType& compType,
							  const value_type min = std::numeric_limits<value_type>::min(),
							  const value_type max = std::numeric_limits<value_type>::max())
{
	srand(filterValueSeed);
	value_type v = 0;

	for (size_t i = 1; i <= numTests; ++i)
	{
		v = (value_type)rand() % (max - min) + min;
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i;
		BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
		auto q = dummy::exeIndexFilterQry(arrName, compType, v);
		BOOST_LOG_TRIVIAL(info) << "##################################################";
		_MSDB_TRY_BEGIN
		{
			auto qry = q.getQueryObj();
			dummy::tearDownQuery(qry, filterExpId, i, qry->getArrayDesc()->id_, 0);
		}
			_MSDB_CATCH_ALL
		{
			BOOST_LOG_TRIVIAL(error) << "experimentIndexFilterQry::Query recording error";
		}
	}
}
}		// experiments
}		// msdb
#endif	// _MSDB_EXPERIMENTS_FILTER_H_