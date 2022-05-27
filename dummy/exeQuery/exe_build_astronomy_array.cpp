#include "exe_build_astronomy_array.h"
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <dummy_astronomy_array_3d.h>
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
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
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
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
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
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
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
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
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
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
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
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
}		// data_lunar102400x40960

namespace data_star64x64x64
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
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
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star64x64x64::getBuildIndexAFL(compType, indexType));
	BOOST_LOG_TRIVIAL(info) << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	BOOST_LOG_TRIVIAL(info) << qry.strStatus() << std::endl;
	BOOST_LOG_TRIVIAL(info) << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
}		// data_star64x64x64
}		// dummy
}		// msdb