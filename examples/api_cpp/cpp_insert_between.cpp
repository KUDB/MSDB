#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy/dummy.h>
#include <array/arrayMgr.h>

int main()
{
	auto arrDesc = msdb::dummy::star1024x1024::getDummyArrayDesc();
	msdb::core::arrayMgr::instance()->registArray(arrDesc);

	msdb::Context ctx;

	//auto afl = msdb::Between(
	//	msdb::Filter(
	//		msdb::Insert(
	//			msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//			msdb::dummy::star1024x1024::filePath
	//		),
	//		msdb::Attribute("ATTR_1") <= 10
	//	),
	//	msdb::Domain(msdb::Coordinate({ 128 - 2, 128 - 2 }), msdb::Coordinate({ 128 + 2, 128 + 2 }))
	//);

	//auto afl = msdb::Filter(
	//	msdb::Insert(
	//		msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//		msdb::dummy::star1024x1024::filePath
	//	),
	//	msdb::Attribute("ATTR_1") == 4
	//);

	//{
	//	//auto afl = msdb::Between(
	//	//	msdb::Insert(
	//	//		msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//	//		msdb::dummy::star1024x1024::filePath
	//	//	),
	//	//	msdb::Domain(msdb::Coordinate({ 0, 0 }), msdb::Coordinate({ 4, 4 }))
	//	//);

	//	auto afl = msdb::Between(
	//		msdb::Filter(
	//			msdb::Insert(
	//				msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//				msdb::dummy::star1024x1024::filePath
	//			),
	//			msdb::Attribute("ATTR_1") <= 10
	//		),
	//		msdb::Domain(msdb::Coordinate({ 128 - 2, 128 - 2 }), msdb::Coordinate({ 128 + 2, 128 + 2 }))
	//	);

	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);
	//	
	//	std::cout << qry.strStatus() << std::endl;
	//	std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//}
	//{
	//	auto afl = msdb::Between(
	//		msdb::Insert(
	//			msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//			msdb::dummy::star1024x1024::filePath
	//		),
	//		msdb::Domain(msdb::Coordinate({ 128, 128 }), msdb::Coordinate({ 128 + 4, 128 + 4 }))
	//	);

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);

	//	std::cout << qry.strStatus() << std::endl;
	//	std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//}

	{
		msdb::Context ctx;
		auto afl = msdb::Build(
			0, msdb::dummy::star1024x1024::arrName,
			{
				msdb::DefDimension("Y", 0, 1024, 128, 32),
				msdb::DefDimension("X", 0, 1024, 128, 32)
			},
		{
			msdb::DefAttribute("ATTR_1", msdb::core::eleType::CHAR)
		}
		);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		//ra.close();

		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	{
		//auto afl = msdb::Between(
		//	msdb::Insert(
		//		msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
		//		msdb::dummy::star1024x1024::filePath
		//	),
		//	msdb::Domain(msdb::Coordinate({ 0, 0 }), msdb::Coordinate({ 4, 4 }))
		//);

		auto afl = msdb::Save(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
				msdb::dummy::star1024x1024::filePath
			)
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		ra.close();
		//msdb::printResultArray(ra);

		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}

	{
		auto afl = msdb::Between(
			msdb::Load(
				msdb::Array(ctx, msdb::dummy::star1024x1024::arrName)
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