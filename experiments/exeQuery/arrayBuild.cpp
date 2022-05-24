#include <pch_experiments.h>

#include "arrayBuild.h"
#include <dummy.h>
#include <dummy_astronomy_array.h>
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
	auto afl = msdb::dummy::data_star1024x1024::getArrayBuildAFL(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_star1024x1024::getBuildIndexAFL(
			core::compressionType::SEACOW, core::attrIndexType::MMT));
	std::cout << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;

	return qry;
}
}
namespace data_saturn1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_saturn1024x1024::getArrayBuildAFL(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_saturn1024x1024::getBuildIndexAFL(
			core::compressionType::SEACOW, core::attrIndexType::MMT));
	std::cout << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;

	return qry;
}
}
namespace data_solar1024x1024
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_solar1024x1024::getArrayBuildAFL(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_solar1024x1024::getBuildIndexAFL(
			core::compressionType::SEACOW, core::attrIndexType::MMT));
	std::cout << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;

	return qry;
}
}
namespace data_mars4096x2048
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_mars4096x2048::getArrayBuildAFL(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mars4096x2048::getBuildIndexAFL(
			core::compressionType::SEACOW, core::attrIndexType::MMT));
	std::cout << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;

	return qry;
}
}
namespace data_mercury20480x10240
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_mercury20480x10240::getArrayBuildAFL(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_mercury20480x10240::getBuildIndexAFL(
			core::compressionType::SEACOW, core::attrIndexType::MMT));
	std::cout << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;

	return qry;
}
}
namespace data_lunar102400x40960
{
msdb::Query executeBuildArray(core::materializedType matType, core::compressionType compType)
{
	msdb::Context ctx;
	auto afl = msdb::dummy::data_lunar102400x40960::getArrayBuildAFL(
		core::materializedType::FLATTEN, core::compressionType::SEACOW);

	std::cout << "=====" << std::endl;
	std::cout << afl->toString(0) << std::endl;
	std::cout << "=====" << std::endl;

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;
	std::cout << qry.getTimer()->getDetailResult() << std::endl;

	return qry;
}
msdb::Query executeBuildIndex(core::compressionType compType, core::attrIndexType indexType)
{
	auto afl = msdb::Consume(
		msdb::dummy::data_lunar102400x40960::getBuildIndexAFL(
			core::compressionType::SEACOW, core::attrIndexType::MMT));
	std::cout << afl->toString(0) << std::endl;
	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	std::cout << qry.strStatus() << std::endl;

	return qry;
}
}
}		// dummy
}		// msdb