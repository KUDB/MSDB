#include <pch_experiments.h>
#include "arraySaveLoad.h"

namespace msdb
{
namespace dummy
{
msdb::Query executeInsertSaveArray(const std::string& arrName, const std::string& filePath, const core::compressionType& compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::getInsertSaveAFL(
		msdb::dummy::data_star1024x1024::arrName,
		msdb::dummy::data_star1024x1024::filePath,
		msdb::core::compressionType::SEACOW);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	ra.close();
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeLoadArray(const std::string& arrName, const core::compressionType& compType)
{
	msdb::Context ctx;
	auto afl = msdb::Consume(msdb::dummy::getLoadAFL(arrName, compType));

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	ra.close();
	if (qry.getStatus() == msdb::Query::Status::FAIL)
	{
		std::cout << ra.getQuery()->getErrorMsg() << std::endl;;
	}
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeRangeQryArray(const std::string& arrName, const core::compressionType& compType, const msdb::Domain& range)
{
	msdb::Context ctx;
	auto afl = msdb::Between(
		msdb::dummy::getLoadAFL(arrName, compType),
		range);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	msdb::printResultArray(ra);
	if (qry.getStatus() == msdb::Query::Status::FAIL)
	{
		std::cout << ra.getQuery()->getErrorMsg() << std::endl;;
	}
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
}		// dummy
}		// msdb