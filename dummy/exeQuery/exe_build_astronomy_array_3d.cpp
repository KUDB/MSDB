#include "exe_build_astronomy_array.h"
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <dummy_astronomy_array_3d.h>
#include <dummy_query_util.h>
#include <api_cpp/msdb.h>

namespace msdb
{
namespace dummy
{
namespace data_star64x64x64
{
msdb::Query executeBuildArray(core::materializedType matType, core::encodingType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_star64x64x64::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star64x64x64::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star64x64x64::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star64x64x64::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_star64x64x64
namespace data_nexrad_16x1024x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::encodingType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_nexrad_16x1024x2048::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_nexrad_16x1024x2048::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_nexrad_16x1024x2048::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_nexrad_16x1024x2048::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_nexrad_16x1024x2048
namespace data_solar_sdo_1024x1024x512
{
msdb::Query executeBuildArray(core::materializedType matType, core::encodingType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_solar_sdo_1024x1024x512::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_solar_sdo_1024x1024x512::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_solar_sdo_1024x1024x512::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_solar_sdo_1024x1024x512::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_solar_sdo_1024x1024x512
namespace data_nexrad_201711_64x24x1024x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::encodingType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_nexrad_201711_64x24x1024x2048::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_nexrad_201711_64x24x1024x2048::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_nexrad_201711_64x24x1024x2048::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::encodingType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_nexrad_201711_64x24x1024x2048::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_nexrad_201711_64x24x1024x2048
}		// dummy
}		// msdb