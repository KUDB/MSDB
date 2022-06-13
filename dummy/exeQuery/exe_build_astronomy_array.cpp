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
namespace data_star1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_star1024x1024::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star1024x1024::getBuildIndexAFL(compType, indexType));
	
	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star1024x1024::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star1024x1024::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_star1024x1024

namespace data_saturn1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_saturn1024x1024::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_saturn1024x1024::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_saturn1024x1024::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_saturn1024x1024::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_saturn1024x1024

namespace data_solar1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_solar1024x1024::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_solar1024x1024::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_solar1024x1024::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_solar1024x1024::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_solar1024x1024

namespace data_mars4096x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_mars4096x2048::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mars4096x2048::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mars4096x2048::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mars4096x2048::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_mars4096x2048

namespace data_mercury20480x10240
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_mercury20480x10240::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mercury20480x10240::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mercury20480x10240::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mercury20480x10240::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_mercury20480x10240

namespace data_lunar102400x40960
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_lunar102400x40960::getArrayBuildAFL(matType, compType);

	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	BOOST_LOG_TRIVIAL(info) << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_lunar102400x40960::getBuildIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeSaveIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_lunar102400x40960::getSaveIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
msdb::Query executeLoadIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_lunar102400x40960::getLoadIndexAFL(compType, indexType));

	return exeQuery_print_Timer(afl);
}
}		// data_lunar102400x40960
}		// dummy
}		// msdb