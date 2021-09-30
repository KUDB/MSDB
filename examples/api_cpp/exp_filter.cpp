#include "exp_filter.h"
#include <dummy.h>

namespace msdb
{
namespace experiments
{
std::shared_ptr<FilterOpr> makeFilterEqQry(std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value)
{
	return Filter(opt, Attribute(attrName) == value);
}

std::shared_ptr<IndexFilterOpr> makeIndexFilterEqQry(std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value)
{
	return IndexFilter(opt, Attribute(attrName) == value);
}

std::shared_ptr<FilterOpr> makeFilterLessQry(std::shared_ptr<AFLOperator> opt, std::string attrName, int64_t value)
{
	return Filter(opt, Attribute(attrName) < value);
}
std::vector<core::pTimer> exeFilterTest(
	const int numTest, std::shared_ptr<AFLOperator> childQry, std::string attrName)
{
	srand(filterValueSeed);
	std::vector<core::pTimer> result;
	for (int i = 0; i < numTest; ++i)
	{
		int64_t v = (unsigned char)rand() % valueLimit;
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
		dummy::tearDownQuery(ra.getQuery(), filterExpId, i, ra.getArrayDesc()->id_, 0);
	}
	return result;
}
std::vector<core::pTimer> exeIndexFilterTest(
	const int numTest, std::shared_ptr<AFLOperator> childQry, std::string attrName)
{
	srand(filterValueSeed);
	std::vector<core::pTimer> result;
	for (int i = 0; i < numTest; ++i)
	{
		int64_t v = (unsigned char)rand() % valueLimit;
		auto afl = Consume(makeIndexFilterEqQry(childQry, attrName, v));

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = Query(afl);
		auto ra = qry.execute();
		//msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;

		//result.push_back(ra.getQuery()->getTimer());
		dummy::tearDownQuery(ra.getQuery(), filterExpId, i, ra.getArrayDesc()->id_, 0);
	}
	return result;
}

namespace data_star1024x1024
{
std::vector<core::pTimer> exeFilterTest(const int numTest, compressionType compType)
{
	auto childQry = dummy::data_star1024x1024::getLoadAFL(compType);

	switch (compType)
	{
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		return experiments::exeIndexFilterTest(numTest, childQry, dummy::data_star1024x1024::nameAttr_0);
	}
	default:
	{
		return experiments::exeFilterTest(numTest, childQry, dummy::data_star1024x1024::nameAttr_0);
	}
	}
}
}		// data_star1024x1024

namespace data_solar1024x1024
{
std::vector<core::pTimer> exeFilterTest(const int numTest, compressionType compType)
{
	auto childQry = dummy::data_solar1024x1024::getLoadAFL(compType);

	switch (compType)
	{
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		return experiments::exeIndexFilterTest(numTest, childQry, dummy::data_solar1024x1024::nameAttr_0);
	}
	default:
	{
		return experiments::exeFilterTest(numTest, childQry, dummy::data_solar1024x1024::nameAttr_0);
	}
	}
}
}		// data_solar1024x1024

namespace data_saturn1024x1024
{
std::vector<core::pTimer> exeFilterTest(const int numTest, compressionType compType)
{
	auto childQry = dummy::data_saturn1024x1024::getLoadAFL(compType);

	switch (compType)
	{
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		return experiments::exeIndexFilterTest(numTest, childQry, dummy::data_saturn1024x1024::nameAttr_0);
	}
	default:
	{
		return experiments::exeFilterTest(numTest, childQry, dummy::data_saturn1024x1024::nameAttr_0);
	}
	}
}
}		// data_saturn1024x1024

namespace data_mars4096x2048
{
std::vector<core::pTimer> exeFilterTest(const int numTest, compressionType compType)
{
	auto childQry = dummy::data_mars4096x2048::getLoadAFL(compType);

	switch (compType)
	{
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		return experiments::exeIndexFilterTest(numTest, childQry, dummy::data_mars4096x2048::nameAttr_0);
	}
	default:
	{
		return experiments::exeFilterTest(numTest, childQry, dummy::data_mars4096x2048::nameAttr_0);
	}
	}
}
}		// data_mars4096x2048

namespace data_mercury20480x10240
{
std::vector<core::pTimer> exeFilterTest(const int numTest, compressionType compType)
{
	auto childQry = dummy::data_mercury20480x10240::getLoadAFL(compType);

	switch (compType)
	{
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		return experiments::exeIndexFilterTest(numTest, childQry, dummy::data_mercury20480x10240::nameAttr_0);
	}
	default:
	{
		return experiments::exeFilterTest(numTest, childQry, dummy::data_mercury20480x10240::nameAttr_0);
	}
	}
}
}		// data_mercury20480x10240

namespace data_lunar102400x40960
{
std::vector<core::pTimer> exeFilterTest(const int numTest, compressionType compType)
{
	auto childQry = dummy::data_lunar102400x40960::getLoadAFL(compType);

	switch (compType)
	{
	case compressionType::SEACOW:
	case compressionType::SEACOW_HUFFMAN:
	{
		return experiments::exeIndexFilterTest(numTest, childQry, dummy::data_lunar102400x40960::nameAttr_0);
	}
	default:
	{
		return experiments::exeFilterTest(numTest, childQry, dummy::data_lunar102400x40960::nameAttr_0);
	}
	}
}
}		// data_lunar102400x40960
}		// experiments
}		// msdb