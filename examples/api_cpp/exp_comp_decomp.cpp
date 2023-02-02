#include <iostream>
#include <thread>

#include <api_cpp/msdb.h>
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <array/arrayMgr.h>

#include <index/mmt.h>
#include <index/compass.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/seacow/seChunk.h>

int main()
{
	//////////////////////////////////////////////////
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
		auto afl = msdb::Build(
			msdb::dummy::data_star1024x1024::aid + msdb::dummy::arr_id_seacow, 
			msdb::dummy::data_star1024x1024::arrName + "_SEACOW",
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
		auto afl = msdb::Consume(
			msdb::dummy::data_star1024x1024::getBuildIndexAFL(
				msdb::encodingType::SEACOW, msdb::attrIndexType::MMT));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	////////////////////////////////////////////////
	{
		msdb::Context ctx;
		auto afl = msdb::Comp(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
				msdb::dummy::data_star1024x1024::filePath
			),
			msdb::encodingType::SEACOW,
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
		//auto afl = msdb::Consume(
		//	msdb::Decomp(
		//		msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
		//		msdb::encodingType::SEACOW,
		//		msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
		//	)
		//);
		auto afl = msdb::Between(
			msdb::Decomp(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName + "_SEACOW"),
				msdb::encodingType::SEACOW,
				msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
			),
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

	return 0;
}