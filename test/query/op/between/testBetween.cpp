#include <pch_test.h>
#include <dummy_array_in_memory.h>
#include <dummy_array_util.h>
#include <api_cpp_operator/cpp_io_operators.h>
#include <op/between/between_plan.h>
#include <op/insert/insert_plan.h>
#include <random>

using namespace msdb::core;
using namespace msdb::dummy;

TEST(query_op_between, array_uint8_2_16x16_4x4)
{
	msdb::Context ctx;
	const size_t dataSize = 16 * 16;

	//////////////////////////////////////////////////
	// Build array
	EXPECT_EQ(array_uint8_2_16x16_4x4::buildArray(), msdb::Query::Status::COMPLETE);

	//////////////////////////////////////////////////
	// Insert data
	EXPECT_EQ(array_uint8_2_16x16_4x4::insertTestData(), msdb::Query::Status::COMPLETE);
	
	//////////////////////////////////////////////////
	// Datasets
	auto attrs = array_uint8_2_16x16_4x4::generateTestData();
	auto pParam_0 = std::static_pointer_cast<opParamMemory::paramType>(attrs[0].getParam());
	auto pParam_1 = std::static_pointer_cast<opParamMemory::paramType>(attrs[1].getParam());
	auto pDataA_0 = std::static_pointer_cast<std::array<uint8_t, dataSize>>(std::get<0>(*pParam_0));
	auto pDataA_1 = std::static_pointer_cast<std::array<uint8_t, dataSize>>(std::get<0>(*pParam_1));

	auto comp_attr_00 = std::array<uint8_t, dataSize>();
	auto comp_attr_01 = std::array<uint8_t, dataSize>();

	{
		// Comparison data
		comp_attr_00.fill(0);
		comp_attr_01.fill(0);
		//int x[2] = { 8, 11 };
		//int y[2] = { 5, 11 };

		int x[2] = { 8, 11 };
		int y[2] = { 5, 11 };

		if (x[0] > x[1])	std::swap(x[0], x[1]);
		if (y[0] > y[1])	std::swap(y[0], y[1]);

		for (int i = y[0]; i <= y[1]; ++i)
		{
			for (int j = x[0]; j <= x[1]; ++j)
			{
				comp_attr_00[i * 16 + j] = pDataA_0->at(i * 16 + j);
				comp_attr_01[i * 16 + j] = pDataA_1->at(i * 16 + j);
			}
		}

		// Query Execute
		auto betweenAfl = msdb::Between(msdb::Array(ctx, array_uint8_2_16x16_4x4::arrName), msdb::Domain({ y[0], x[0] }, { y[1], x[1] }));
		BOOST_LOG_TRIVIAL(debug) << betweenAfl->toString(0);
		auto qry = msdb::Query(betweenAfl);
		qry.setRawResultOut();
		auto ra = qry.execute();

		EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);

		auto out = qry.getRawResult();
		EXPECT_TRUE(out->isEqual(0, comp_attr_00.data(), dataSize));
		EXPECT_TRUE(out->isEqual(1, comp_attr_01.data(), dataSize));
		if (!out->isEqual(0, comp_attr_00.data(), dataSize) || !out->isEqual(1, comp_attr_01.data(), dataSize))
		{
			out->print();
		}
	}

	//////////////////////////////////////////////////
	// Between Test
	srand(0);
	for (int tc = 0; tc < 10; ++tc)
	{
		// Comparison data
		comp_attr_00.fill(0);
		comp_attr_01.fill(0);
		int x[2] = { rand() % 16, rand() % 16 };
		int y[2] = { rand() % 16, rand() % 16 };
		
		if (x[0] > x[1])	std::swap(x[0], x[1]);
		if (y[0] > y[1])	std::swap(y[0], y[1]);

		for (int i = y[0]; i <= y[1]; ++i)
		{
			for (int j = x[0]; j <= x[1]; ++j)
			{
				comp_attr_00[i * 16 + j] = pDataA_0->at(i * 16 + j);
				comp_attr_01[i * 16 + j] = pDataA_1->at(i * 16 + j);
			}
		}

		// Query Execute
		auto betweenAfl = msdb::Between(msdb::Array(ctx, array_uint8_2_16x16_4x4::arrName), msdb::Domain({y[0], x[0]}, {y[1], x[1]}));
		BOOST_LOG_TRIVIAL(debug) << betweenAfl->toString(0);
		auto qry = msdb::Query(betweenAfl);
		qry.setRawResultOut();
		auto ra = qry.execute();

		EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);

		auto out = qry.getRawResult();
		EXPECT_TRUE(out->isEqual(0, comp_attr_00.data(), dataSize));
		EXPECT_TRUE(out->isEqual(1, comp_attr_01.data(), dataSize));
		if (!out->isEqual(0, comp_attr_00.data(), dataSize) || !out->isEqual(1, comp_attr_01.data(), dataSize))
		{
			out->print();
		}
	}
}