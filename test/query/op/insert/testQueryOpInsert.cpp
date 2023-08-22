#include <pch_test.h>
#include <dummy_array_in_memory.h>
#include <dummy_array_util.h>
#include <api_cpp_operator/cpp_io_operators.h>
#include <op/insert/insert_plan.h>

using namespace msdb::core;
using namespace msdb::dummy;
TEST(query_op_insert, from_mem_array_uint8_2_16x16_4x4)
{
	msdb::Context ctx;
	const uint64_t dataSize = 16 * 16;

	//////////////////////////////////////////////////
	// Build array
	EXPECT_TRUE(array_uint8_2_16x16_4x4::buildArray() == msdb::Query::Status::COMPLETE);

	//////////////////////////////////////////////////
	// Test data generate
	auto attrs = array_uint8_2_16x16_4x4::generateTestData();

	//////////////////////////////////////////////////
	// Insert data and Save an array
	{
		auto insertAfl = msdb::Save(
			msdb::Insert(
				msdb::Array(ctx, array_uint8_2_16x16_4x4::arrName),
				attrs
			)
		);
		std::cout << "=====" << std::endl;
		std::cout << insertAfl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(insertAfl);
		qry.setRawResultOut();
		qry.setVerbose();
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
	//////////////////////////////////////////////////
	{
		auto pParam_0 = std::static_pointer_cast<opParamMemory::paramType>(attrs[0].getParam());
		auto pParam_1 = std::static_pointer_cast<opParamMemory::paramType>(attrs[1].getParam());
		auto pDataA_0 = std::static_pointer_cast<std::array<uint8_t, dataSize>>(std::get<0>(*pParam_0));
		auto pDataA_1 = std::static_pointer_cast<std::array<uint8_t, dataSize>>(std::get<0>(*pParam_1));

		auto loadAfl = msdb::Load(
			msdb::Array(ctx, array_uint8_2_16x16_4x4::arrName)
		);
		std::cout << "=====" << std::endl;
		std::cout << loadAfl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(loadAfl);
		qry.setRawResultOut();
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		msdb::printResultArray(ra);

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);

		auto out = qry.getRawResult();
		out->print();
		EXPECT_TRUE(out->isEqual(0, (void*)pDataA_0->data(), pDataA_0->size()));
		//EXPECT_TRUE(out->isEqual(1, (void*)pDataA_1->data(), pDataA_1->size()));
	}
}
TEST(query_op_insert, from_mem_array_mem_char_4x4)
{
	msdb::Context ctx;
	const uint64_t dataSize = 16;

	//////////////////////////////////////////////////
	// Build array
	EXPECT_TRUE(array_mem_char_4x4::buildArray() == msdb::Query::Status::COMPLETE);

	//////////////////////////////////////////////////
	// Test data generate
	auto pData_0 = std::make_shared<std::array<char, dataSize>>();
	auto pData_1 = std::make_shared<std::array<char, dataSize>>();
	auto pData_2 = std::make_shared<std::array<char, dataSize>>();
	for (int i = 0; i < dataSize; ++i)
	{
		pData_0->at(i) = i;
		pData_1->at(i) = i * i;
		pData_2->at(i) = i * (-1);
	}

	std::map<attributeId, msdb::core::opParamMemory> attrs;
	attrs[0] = msdb::core::opParamMemory(pData_0, dataSize * sizeof(char));
	attrs[1] = msdb::core::opParamMemory(pData_1, dataSize * sizeof(char));
	attrs[2] = msdb::core::opParamMemory(pData_2, dataSize * sizeof(char));
	//////////////////////////////////////////////////
	{
		auto insertAfl = msdb::Save(
			msdb::Insert(
				msdb::Array(ctx, array_mem_char_4x4::arrName),
				attrs
			)
		);
		std::cout << "=====" << std::endl;
		std::cout << insertAfl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(insertAfl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
	{
		auto loadAfl = msdb::Load(
			msdb::Array(ctx, array_mem_char_4x4::arrName)
		);
		std::cout << "=====" << std::endl;
		std::cout << loadAfl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(loadAfl);
		qry.setRawResultOut();
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		msdb::printResultArray(ra);

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);

		auto out = qry.getRawResult();
		EXPECT_TRUE(out->isEqual(0, (void*)pData_0->data(), pData_0->size()));
		EXPECT_TRUE(out->isEqual(1, (void*)pData_1->data(), pData_1->size()));
		EXPECT_TRUE(out->isEqual(2, (void*)pData_2->data(), pData_2->size()));

		EXPECT_FALSE(out->isEqual(0, (void*)pData_1->data(), pData_1->size()));
		EXPECT_FALSE(out->isEqual(1, (void*)pData_2->data(), pData_2->size()));
		EXPECT_FALSE(out->isEqual(2, (void*)pData_0->data(), pData_0->size()));
	}
}