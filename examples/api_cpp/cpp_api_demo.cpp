#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy.h>
#include <array/arrayMgr.h>
#include <thread>
#include <windows.h>

int main()
{
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Build(
			0, msdb::dummy::data_star1024x1024::arrName,
			{
				msdb::DefDimension("Y", 0, 1024, 128, 32),
				msdb::DefDimension("X", 0, 1024, 128, 32)
			},
			{
				msdb::DefAttribute("ATTR_1", msdb::core::eleType::CHAR)
			});
		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Save(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName),
				msdb::dummy::data_star1024x1024::filePath
			)
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		ra.close();
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Load(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName)
			),
			msdb::Domain(msdb::Coordinate({ 0, 0 }), msdb::Coordinate({ 4, 4 }))
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Load(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName)
			),
			msdb::Domain(msdb::Coordinate({ 100, 100 }), msdb::Coordinate({ 104, 104 }))
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Filter(
				msdb::Load(
					msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName)
				),
				msdb::Attribute("ATTR_1") > 10),
				msdb::Domain(msdb::Coordinate({ 100, 100 }), msdb::Coordinate({ 104, 104 }))
			);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Filter(
				msdb::Load(
					msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName)
				),
				msdb::Attribute("ATTR_1") >= 7),
			msdb::Domain(msdb::Coordinate({ 100, 100 }), msdb::Coordinate({ 104, 104 }))
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Filter(
				msdb::Load(
					msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName)
				),
				msdb::Attribute("ATTR_1") == 7),
			msdb::Domain(msdb::Coordinate({ 100, 100 }), msdb::Coordinate({ 104, 104 }))
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////////////////////////

	return 0;
}