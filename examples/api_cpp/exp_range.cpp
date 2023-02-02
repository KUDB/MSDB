#pragma once
#include "exp_range.h"
#include "exp_filter.h"
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <api_cpp_operator/cpp_io_operators.h>

namespace msdb
{
namespace experiments
{
Domain getRandomRange(const uint64_t dimX, const uint64_t dimY, const float selectivity)
{
	int area = dimX * dimY;
	float selectedArea = area * selectivity / 100.0;

	int width = sqrt(selectivity / 100.0) * dimX, height = sqrt(selectivity / 100.0) * dimY;
	int spX = rand() % (size_t)(dimX - width), spY = rand() % (size_t)(dimY - height);

	int epX = spX + width;
	int epY = spY + height;
	int cal = (epX - spX) * (epY - spY);

	Coordinates sp(core::coor({ spY, spX }));
	Coordinates ep(core::coor({ spY + height, spX + width }));

	return Domain(sp, ep);
}
std::shared_ptr<BetweenOpr> makeRangeFilterEqQry(std::shared_ptr<AFLOperator> opt, std::string attrName, Domain d, int64_t value)
{
	return Between(makeFilterEqQry(opt, attrName, value), d);
}
std::shared_ptr<BetweenOpr> makeRangeIndexFilterEqQry(std::shared_ptr<AFLOperator> opt, std::string attrName, Domain d, int64_t value)
{
	return Between(makeIndexFilterEqQry(opt, attrName, value), d);
}
std::vector<core::pTimer> exeRangeFilterTest(const int numTest, std::shared_ptr<AFLOperator> childQry, 
											 Coordinates arraySize, std::string attrName)
{
	srand(rangeFilter::filterValueSeed);
	std::vector<core::pTimer> result;

	for (int i = 0; i < rangeFilter::selectivity.size(); ++i)
	{
		int64_t v = (unsigned char)rand() % filter::valueLimit;
		Domain d = getRandomRange(arraySize.getCoor()[1], arraySize.getCoor()[0], rangeFilter::selectivity[i]);
		auto afl = Consume(makeFilterEqQry(childQry, attrName, v));

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = Query(afl);
		auto ra = qry.execute();
		//msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;

		//result.push_back(ra.getQuery()->getTimer());
		dummy::tearDownQuery(qry.getTimer(), filter::filterExpId, i, ra.getArrayDesc()->id_, 0);
	}

	return result;
}

namespace data_star1024x1024
{
std::vector<core::pTimer> exeRangeTest(const int numTest, encodingType compType)
{
	auto childQry = dummy::data_star1024x1024::getLoadAFL(compType);

	switch (compType)
	{
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry, 
			dummy::data_star1024x1024::dims, 
			dummy::data_star1024x1024::nameAttr_0);
	}
	default:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_star1024x1024::dims,
			dummy::data_star1024x1024::nameAttr_0);
	}
	}
}
}
namespace data_solar1024x1024
{
std::vector<core::pTimer> exeRangeTest(const int numTest, encodingType compType)
{
	auto childQry = dummy::data_solar1024x1024::getLoadAFL(compType);

	switch (compType)
	{
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_solar1024x1024::dims,
			dummy::data_solar1024x1024::nameAttr_0);
	}
	default:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_solar1024x1024::dims,
			dummy::data_solar1024x1024::nameAttr_0);
	}
	}
}
}
namespace data_saturn1024x1024
{
std::vector<core::pTimer> exeRangeTest(const int numTest, encodingType compType)
{
	auto childQry = dummy::data_saturn1024x1024::getLoadAFL(compType);

	switch (compType)
	{
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_saturn1024x1024::dims,
			dummy::data_saturn1024x1024::nameAttr_0);
	}
	default:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_saturn1024x1024::dims,
			dummy::data_saturn1024x1024::nameAttr_0);
	}
	}
}
}
namespace data_mars4096x2048
{
std::vector<core::pTimer> exeRangeTest(const int numTest, encodingType compType)
{
	auto childQry = dummy::data_mars4096x2048::getLoadAFL(compType);

	switch (compType)
	{
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_mars4096x2048::dims,
			dummy::data_mars4096x2048::nameAttr_0);
	}
	default:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_mars4096x2048::dims,
			dummy::data_mars4096x2048::nameAttr_0);
	}
	}
}
}
namespace data_mercury20480x10240
{
std::vector<core::pTimer> exeRangeTest(const int numTest, encodingType compType)
{
	auto childQry = dummy::data_mercury20480x10240::getLoadAFL(compType);

	switch (compType)
	{
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_mercury20480x10240::dims,
			dummy::data_mercury20480x10240::nameAttr_0);
	}
	default:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_mercury20480x10240::dims,
			dummy::data_mercury20480x10240::nameAttr_0);
	}
	}
}
}
namespace data_lunar102400x40960
{
std::vector<core::pTimer> exeRangeTest(const int numTest, encodingType compType)
{
	auto childQry = dummy::data_lunar102400x40960::getLoadAFL(compType);

	switch (compType)
	{
	case encodingType::SEACOW:
	case encodingType::SEACOW_HUFFMAN:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_lunar102400x40960::dims,
			dummy::data_lunar102400x40960::nameAttr_0);
	}
	default:
	{
		return experiments::exeRangeFilterTest(
			numTest, childQry,
			dummy::data_lunar102400x40960::dims,
			dummy::data_lunar102400x40960::nameAttr_0);
	}
	}
}
}
}		// experiments
}		// msdb

