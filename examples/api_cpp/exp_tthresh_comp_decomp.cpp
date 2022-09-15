#include <iostream>

#include <api_cpp/msdb.h>
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <array/arrayMgr.h>

int main()
{
	//////////////////////////////
	// Build
	{
		msdb::Context ctx;
		auto afl = msdb::Build(
			msdb::dummy::data_star1024x1024::aid,
			msdb::dummy::data_star1024x1024::arrName,
			{
				msdb::DefDimension("Y", 0, 1024, 128, 32),
				msdb::DefDimension("X", 0, 1024, 128, 32)
			},
			{
				msdb::DefAttribute(
					"ATTR_1", msdb::core::concreteTy<char>(),
					msdb::materializedType::FLATTEN, msdb::encodingType::SEACOW,
					{
						std::make_pair<>(_STR_PARAM_WAVELET_LEVEL_, std::to_string(msdb::dummy::data_star1024x1024::wtLevel)),
						std::make_pair<>(_STR_PARAM_SE_LEVEL_, std::to_string(msdb::dummy::data_star1024x1024::wtLevel)),
						std::make_pair<>(_STR_PARAM_MMT_LEVEL_, std::to_string(msdb::dummy::data_star1024x1024::mmtLevel)),
						std::make_pair<>(_STR_PARAM_COMPASS_BINS_, std::to_string(msdb::dummy::data_star1024x1024::compassBins))
					})
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
		auto afl = msdb::dummy::data_star1024x1024::getArrayBuildAFL(
			msdb::core::materializedType::FLATTEN, msdb::core::encodingType::TTHRESH);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		std::cout << qry.getTimer()->getDetailResult() << std::endl;
	}
	//////////////////////////////

	// Insert
	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::dummy::getInsertSaveAFL(
	//		msdb::dummy::data_star1024x1024::arrName,
	//		msdb::dummy::data_star1024x1024::filePath,
	//		msdb::core::encodingType::TTHRESH);

	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	ra.close();
	//	std::cout << qry.strStatus() << std::endl;
	//	std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//}
	//////////////////////////////
	// Load
	////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName),
				msdb::dummy::data_star1024x1024::filePath
			),
			msdb::Domain(msdb::Coordinates({ 500, 500 }), msdb::Coordinates({ 505, 505 }))
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
	////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::dummy::getLoadAFL(
				msdb::dummy::data_star1024x1024::arrName,
				msdb::core::encodingType::TTHRESH),
			msdb::Domain(msdb::Coordinates({ 500, 500 }), msdb::Coordinates({ 505, 505 }))
		);

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
	}
	//////////////////////////////

	return 0;
}
