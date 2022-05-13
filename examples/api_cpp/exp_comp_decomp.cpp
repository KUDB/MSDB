#include <iostream>
#include <thread>

#include <api_cpp/msdb.h>
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <array/arrayMgr.h>

#include <index/mmt.h>
#include <index/compass.h>
#include <op/wavelet_encode/wtChunk.h>
#include <op/se_compression/seChunk.h>

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
				msdb::DefAttribute(
					"ATTR_1", msdb::core::concreteTy<char>(), 
					msdb::materializedType::FLATTEN, msdb::compressionType::SEACOW,
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
		auto afl = msdb::Comp(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName),
				msdb::dummy::data_star1024x1024::filePath
			),
			msdb::compressionType::SEACOW,
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
				msdb::compressionType::SEACOW,
				msdb::dummy::data_star1024x1024::wtLevel, msdb::dummy::data_star1024x1024::mmtLevel
			),
			msdb::Domain(msdb::Coordinates({ 0, 0 }), msdb::Coordinates({ 4, 4 }))
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