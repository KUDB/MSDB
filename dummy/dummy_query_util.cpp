#include <dummy_query_util.h>
#include <dummy_array_util.h>

namespace msdb
{
namespace dummy
{
msdb::Query exeQuery(std::shared_ptr<AFLOperator> afl, bool printAFL, bool printResult, bool printTimer)
{
	msdb::Context ctx;

	if (printAFL)
	{
		BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
		BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
		BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	}

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();

	if (printResult)
	{
		msdb::printResultArray(ra);
	}

	if (qry.getStatus() == msdb::Query::Status::FAIL)
	{
		BOOST_LOG_TRIVIAL(info) << ra.getQuery()->getErrorMsg() << std::endl;;
	}
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	if (printTimer)
	{
		BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;
	}

	return qry;
}
msdb::Query exeQuery_print_Timer(std::shared_ptr<AFLOperator> afl)
{
	return exeQuery(afl, true, false, true);
}
msdb::Query exeQuery_print_Result_Timer(std::shared_ptr<AFLOperator> afl)
{
	return exeQuery(afl, true, true, true);
}

////////////////////////////////////////
// Range
////////////////////////////////////////
msdb::Query exeRangeQry(const std::string& arrName, const core::compressionType& compType, 
						const msdb::Domain& range)
{
	auto afl = msdb::Consume(
		msdb::Between(
			msdb::dummy::getLoadAFL(arrName, compType),
		range));

	return exeQuery(afl);
}
msdb::Query exeRangeQryPrintResult(const std::string& arrName, const core::compressionType& compType,
								   const msdb::Domain& range)
{
	auto afl = msdb::Between(
			msdb::dummy::getLoadAFL(arrName, compType),
			range);

	return exeQuery_print_Result_Timer(afl);
}

////////////////////////////////////////
// Filter
////////////////////////////////////////
/* Naive Filter */
msdb::Query exeFilterQry(const std::string& arrName, const core::compressionType& compType,
						 const int64_t value)
{
	auto afl = msdb::Consume(
		msdb::Filter(
			msdb::dummy::getLoadAFL(arrName, compType),
			msdb::Attribute("ATTR_1") == value));

	return exeQuery(afl);
}
msdb::Query exeFilterQryPrintResult(const std::string& arrName, const core::compressionType& compType, 
									const int64_t value)
{
	auto afl = msdb::Filter(
		msdb::dummy::getLoadAFL(arrName, compType),
		msdb::Attribute("ATTR_1") == value);

	return exeQuery_print_Result_Timer(afl);
}
/* Index Filter */
msdb::Query exeIndexFilterQry(const std::string& arrName, const core::compressionType& compType, 
							  const int64_t value)
{
	auto afl = msdb::Consume(
		msdb::IndexFilter(
			msdb::dummy::getLoadAFL(arrName, compType),
			msdb::Attribute("ATTR_1") == value));

	return exeQuery(afl);
}
msdb::Query exeIndexFilterQryPrintResult(const std::string& arrName, const core::compressionType& compType, 
										 const int64_t value)
{
	auto afl = msdb::IndexFilter(
		msdb::dummy::getLoadAFL(arrName, compType),
		msdb::Attribute("ATTR_1") == value);

	return exeQuery_print_Result_Timer(afl);
}
////////////////////////////////////////
// Range-Filter
////////////////////////////////////////
msdb::Query exeRangeFilterQry(const std::string& arrName, const core::compressionType& compType,
							  const msdb::Domain& range, const int64_t value)
{
	auto afl = msdb::Consume(
		msdb::Between(
			msdb::Filter(
				msdb::dummy::getLoadAFL(arrName, compType),
				msdb::Attribute("ATTR_1") == value),
			range));

	return exeQuery(afl);
}
msdb::Query exeRangeFilterQryPrintResult(const std::string& arrName, const core::compressionType& compType, 
										 const msdb::Domain& range, const int64_t value)
{
	auto afl = msdb::Between(
		msdb::Filter(
			msdb::dummy::getLoadAFL(arrName, compType),
			msdb::Attribute("ATTR_1") == value),
		range);

	return exeQuery_print_Result_Timer(afl);
}
/* Index Filter */
msdb::Query exeRangeIndexFilterQry(const std::string& arrName, const core::compressionType& compType, 
								   const msdb::Domain& range, const int64_t value)
{
	auto afl = msdb::Consume(
		msdb::Between(
			msdb::IndexFilter(
				msdb::dummy::getLoadAFL(arrName, compType),
				msdb::Attribute("ATTR_1") == value),
			range));

	return exeQuery(afl);
}
msdb::Query exeRangeIndexFilterQryPrintResult(const std::string& arrName, const core::compressionType& compType,
											  const msdb::Domain& range, const int64_t value)
{
	auto afl = msdb::Between(
		msdb::IndexFilter(
			msdb::dummy::getLoadAFL(arrName, compType),
			msdb::Attribute("ATTR_1") == value),
		range);

	return exeQuery_print_Result_Timer(afl);
}
}		// dummy
}		// msdb