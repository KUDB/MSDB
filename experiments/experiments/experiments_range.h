#pragma once
#ifndef _MSDB_EXPERIMENTS_RANGE_H_
#define _MSDB_EXPERIMENTS_RANGE_H_

#include <pch_experiments.h>
#include <dummy_query_util.h>

namespace msdb
{
namespace experiments
{
static const int rangeFilterValueSeed = 21020;
static const int rangeFilterExpId = 15;
static const int numTests = 3;
static const std::vector<float> rangeSelectivity({0.1, 10, 20, 30, 40});

Domain getRandomRange(const core::coordinates& dims, const float selectivity);

template <typename value_type>
void experimentRangeFilterQry(const std::string& arrName, const core::compressionType& compType,
							  const core::coordinates& dims,
							  const value_type min = std::numeric_limits<value_type>::min(),
							  const value_type max = std::numeric_limits<value_type>::max())
{
	srand(rangeFilterValueSeed);
	value_type v = 0;

	for (size_t j = 0; j < rangeSelectivity.size(); ++j)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			_MSDB_TRY_BEGIN
			{
				v = (value_type)rand() % (max - min) + min;
				Domain r = getRandomRange(dims, rangeSelectivity[j]);
				BOOST_LOG_TRIVIAL(info) << "##################################################";
				BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i + 1;
				BOOST_LOG_TRIVIAL(info) << "# Selectivity: " << rangeSelectivity[j];
				BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
				auto q = dummy::exeRangeFilterQry(arrName, compType, r, v);
				BOOST_LOG_TRIVIAL(info) << "##################################################";

				auto qry = q.getQueryObj();
				dummy::tearDownQuery(qry, rangeFilterExpId, j * numTests + i, qry->getArrayDesc()->id_, 0);
			}
				_MSDB_CATCH_ALL
			{
				BOOST_LOG_TRIVIAL(error) << "experimentRangeFilterQry::Query recording error";
			}
		}
	}

}
template <typename value_type>
void experimentRangeIndexFilterQry(const std::string& arrName, const core::compressionType& compType, 
								   const core::coordinates& dims,
								   const value_type min = std::numeric_limits<value_type>::min(),
								   const value_type max = std::numeric_limits<value_type>::max())
{
	srand(rangeFilterValueSeed);
	value_type v = 0;

	for (size_t j = 0; j < rangeSelectivity.size(); ++j)
	{
		for (size_t i = 0; i < numTests; ++i)
		{
			_MSDB_TRY_BEGIN
			{
				v = (value_type)rand() % (max - min) + min;
				Domain r = getRandomRange(dims, rangeSelectivity[j]);
				BOOST_LOG_TRIVIAL(info) << "##################################################";
				BOOST_LOG_TRIVIAL(info) << "# TEST CASE: " << i + 1;
				BOOST_LOG_TRIVIAL(info) << "# Selectivity: " << rangeSelectivity[j];
				BOOST_LOG_TRIVIAL(info) << "# Find value: " << static_cast<int64_t>(v);
				auto q = dummy::exeRangeIndexFilterQry(arrName, compType, r, v);
				BOOST_LOG_TRIVIAL(info) << "##################################################";

				auto qry = q.getQueryObj();
				dummy::tearDownQuery(qry, rangeFilterExpId, j * numTests + i, qry->getArrayDesc()->id_, 0);
			}
				_MSDB_CATCH_ALL
			{
				BOOST_LOG_TRIVIAL(error) << "experimentRangeIndexFilterQry::Query recording error";
			}
		}
	}
}
}		// experiments
}		// msdb
#endif _MSDB_EXPERIMENTS_RANGE_H_