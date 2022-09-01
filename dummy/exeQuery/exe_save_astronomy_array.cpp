#include "exe_save_astronomy_array.h"

namespace msdb
{
namespace dummy
{
msdb::Query executeInsertSaveArray(const std::string& arrName, const std::string& filePath, const core::encodingType& compType)
{
	msdb::Context ctx;
	auto afl = msdb::Consume(msdb::dummy::getInsertSaveAFL(
		arrName, filePath, compType));

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	// TODO::Remove
	//qry.setVerbose();
	// TODO::Remove
	auto ra = qry.execute();
	ra.close();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	_MSDB_TRY_BEGIN
	{
		int expTrial = 0;
		int method = 0;
		dummy::tearDownQuery(qry.getQueryObj(), saveExpId, expTrial, qry.getQueryObj()->getArrayDesc()->id_, method);
	}
	_MSDB_CATCH_ALL
	{
		BOOST_LOG_TRIVIAL(error) << "experimentIndexFilterQry::Query recording error";
	}

	return qry;
}
msdb::Query executeLoadArray(const std::string& arrName, const core::encodingType& compType)
{
	msdb::Context ctx;
	auto afl = msdb::Consume(msdb::dummy::getLoadAFL(arrName, compType));

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	// TODO::Remove
	//qry.setVerbose();
	// TODO::Remove
	auto ra = qry.execute();
	ra.close();
	if (qry.getStatus() == msdb::Query::Status::FAIL)
	{
		BOOST_LOG_TRIVIAL(info) << ra.getQuery()->getErrorMsg() << std::endl;;
	}
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	_MSDB_TRY_BEGIN
	{
		int expTrial = 0;
		int method = 0;
		dummy::tearDownQuery(qry.getQueryObj(), loadExpId, expTrial, qry.getQueryObj()->getArrayDesc()->id_, method);
	}
	_MSDB_CATCH_ALL
	{
		BOOST_LOG_TRIVIAL(error) << "experimentIndexFilterQry::Query recording error";
	}

	return qry;
}
}		// dummy
}		// msdb