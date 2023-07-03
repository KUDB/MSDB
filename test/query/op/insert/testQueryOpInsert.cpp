#include <pch_test.h>
#include <dummy_array_in_memory.h>
#include <api_cpp_operator/cpp_io_operators.h>
#include <op/insert/insert_plan.h>

using namespace msdb::core;

TEST(query_op_insert, insert_from_memory_twoattr_2d)
{
	msdb::Context ctx;
	auto desc = msdb::dummy::array_mem_twoattr_2d::getDummyArrayDesc();

	//////////////////////////////////////////////////
	{
		auto buildAfl = msdb::dummy::array_mem_twoattr_2d::getArrayBuildAFL();
		auto qry = msdb::Query(buildAfl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		//std::cout << buildQry.getTimer()->getDetailResult() << std::endl;

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
	//////////////////////////////////////////////////
	// Test data generate
	const uint64_t dataSize = 16 * 16;
	auto pDataA_0 = std::make_shared<std::array<uint8_t, dataSize>>();
	auto pDataA_1 = std::make_shared<std::array<uint8_t, dataSize>>();

	for (int i = 0; i < dataSize; ++i)
	{
		pDataA_0->at(i) = i;
		pDataA_1->at(i) = i * 2;
	}

	std::map<attributeId, msdb::core::opParamMemory> attrs;
	attrs[0] = msdb::core::opParamMemory(pDataA_0, dataSize * sizeof(uint8_t));
	attrs[1] = msdb::core::opParamMemory(pDataA_1, dataSize * sizeof(uint8_t));
	//////////////////////////////////////////////////
	{
		auto insertAfl = msdb::Save(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::array_mem_twoattr_2d::arrName),
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
		auto loadAfl = msdb::Load(
			msdb::Array(ctx, msdb::dummy::array_mem_twoattr_2d::arrName)
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
		EXPECT_TRUE(out->isEqual(0, (void*)pDataA_0->data(), pDataA_0->size()));
		EXPECT_TRUE(out->isEqual(1, (void*)pDataA_1->data(), pDataA_1->size()));
	}
}

TEST(query_op_insert, insert_from_memory_4x4)
{
	msdb::Context ctx;
	auto desc = msdb::dummy::array_mem_char_4x4::getDummyArrayDesc();
	const uint64_t dataSize = 16;

	//////////////////////////////////////////////////
	{
		auto buildAfl = msdb::dummy::array_mem_char_4x4::getArrayBuildAFL();
		auto qry = msdb::Query(buildAfl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		//std::cout << buildQry.getTimer()->getDetailResult() << std::endl;

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
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
				msdb::Array(ctx, msdb::dummy::array_mem_char_4x4::arrName),
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
			msdb::Array(ctx, msdb::dummy::array_mem_char_4x4::arrName)
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