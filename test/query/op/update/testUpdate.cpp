#include "gtest/gtest.h"

#include <dummy_small_array.h>
#include <dummy_query_util.h>
#include <op/update/update_plan.h>
#include <api_cpp_operator/cpp_io_operators.h>

namespace msdb
{
namespace test
{
TEST(fast_update, fast_update_8x8)
{
	msdb::Context ctx;

	dummy::array_8x8 arr(std::string("8x8"), 88, core::dimension({ 8, 8 }), core::dimension({ 8, 8 }), 
		{ "x", "y" }, { "attr1" }, 
		{ core::concreteTy<uint8_t>() }, { core::materializedType::FLATTEN }, { core::encodingType::NONE }, 
		{});

	auto buildAfl = arr.getArrayBuildAFL();

	auto qry = msdb::Query(buildAfl);
	auto ra = qry.execute();
}

TEST(fast_update, fast_update_1x32x32)
{
	msdb::Context ctx;

	core::arrayId aid = 13232;
	dummy::array_1x32x32 arr(std::string("1x32x32"), aid, {});

	{
		auto buildAfl = arr.getArrayBuildAFL();
		auto qry = msdb::Query(buildAfl);
		auto ra = qry.execute();
		ra.close();
	}
	
	{
		auto loadAfl = msdb::Load(msdb::Array(ctx, aid));
		auto qry = msdb::Query(loadAfl);
		qry.setRawResultOut();

		auto ra = qry.execute();
		auto outArr = ra.getRawResult();
		outArr->print();
		ra.close();
	}

	{
		auto updateAfl = msdb::FastUpdate(msdb::Array(ctx, aid),
			{ {1, "C:/Workspace/21_MSDB/x64/datasets/cifar/cifar_x_train_0.jpg"} }, msdb::Coordinates({ 0, 0, 0 }), msdb::Coordinates({1, 0, 0}));
		auto qry = msdb::Query(updateAfl);
		auto ra = qry.execute();
		ra.close();
	}

	{
		auto loadAfl = msdb::Load(msdb::Array(ctx, aid));
		auto qry = msdb::Query(loadAfl);
		qry.setRawResultOut();

		auto ra = qry.execute();
		auto outArr = ra.getRawResult();
		outArr->print();
		ra.close();
	}
}
}		// test
}		// msdb