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
				msdb::DefAttribute("ATTR_1", msdb::eleType::CHAR, msdb::compressionType::SEACOW)
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
		auto afl = msdb::Comp(
			msdb::Insert(
			msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName),
			msdb::dummy::data_star1024x1024::filePath
		),
			msdb::compressionType::COMPASS,
			msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
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
			msdb::Decomp(
			msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName),
			msdb::compressionType::COMPASS,
			msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
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


	return 0;
}