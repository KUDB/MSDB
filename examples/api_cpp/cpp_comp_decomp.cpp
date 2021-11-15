#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy.h>
#include <array/arrayMgr.h>
#include <thread>
#include <windows.h>

int main()
{
	//////////////////////////////////////////////////
	// Array Build
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Build(
			msdb::dummy::data_star1024x1024::aid + msdb::dummy::arr_id_seacow, msdb::dummy::data_star1024x1024::arrName + "_SEACOW",
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
		std::cout << qry.strStatus() << std::endl;
	}
	// -----------------------------------------------
	{
		msdb::Context ctx;
		auto afl = msdb::Build(
			msdb::dummy::data_star1024x1024::aid + msdb::dummy::arr_id_seacow_huffman, msdb::dummy::data_star1024x1024::arrName + "_SEACOW_HUFFMAN",
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
		std::cout << qry.strStatus() << std::endl;
	}
	// -----------------------------------------------
	{
		msdb::Context ctx;
		auto afl = msdb::Build(
			msdb::dummy::data_star1024x1024::aid + msdb::dummy::arr_id_compass, msdb::dummy::data_star1024x1024::arrName + "_COMPASS",
			{
				msdb::DefDimension("Y", 0, 1024, 128, 32),
				msdb::DefDimension("X", 0, 1024, 128, 32)
			},
			{
				msdb::DefAttribute("ATTR_1", msdb::eleType::CHAR, msdb::compressionType::COMPASS)
			});
		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}

	//////////////////////////////////////////////////
	// Index Build
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Consume(
			msdb::BuildIndex(
				msdb::Insert(
					msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
					msdb::dummy::data_star1024x1024::filePath
				),
				msdb::attrIndexType::MMT, msdb::dummy::data_star1024x1024::mmtLevel
			)
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	// -----------------------------------------------
	{
		msdb::Context ctx;
		auto afl = msdb::Consume(
			msdb::BuildIndex(
				msdb::Insert(
					msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_COMPASS"),
					msdb::dummy::data_star1024x1024::filePath
				),
				msdb::attrIndexType::COMPASS, msdb::dummy::data_star1024x1024::compassBins
		)
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}

	//////////////////////////////////////////////////
	// Index Save
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Consume(
			msdb::SaveIndex(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
				msdb::attrIndexType::MMT
			)
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}

	//////////////////////////////////////////////////
	// Index Load
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Consume(
			msdb::LoadIndex(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
				msdb::attrIndexType::MMT
			)
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}

	//////////////////////////////////////////////////
	// Compression
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Consume(msdb::Comp(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
				msdb::dummy::data_star1024x1024::filePath
			), 
			msdb::compressionType::SEACOW,
			msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
		));

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		ra.close();
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	// -----------------------------------------------
	{
		msdb::Context ctx;
		auto afl = msdb::Consume(msdb::Comp(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_COMPASS"),
				msdb::dummy::data_star1024x1024::filePath
			),
			msdb::compressionType::COMPASS,
			msdb::dummy::data_star1024x1024::compassBins
		));

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
	// Between
	//////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Decomp(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
				msdb::compressionType::SEACOW,
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
	// -----------------------------------------------
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Decomp(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_COMPASS"),
				msdb::compressionType::COMPASS,
				msdb::dummy::data_star1024x1024::compassBins
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

	return 0;
}