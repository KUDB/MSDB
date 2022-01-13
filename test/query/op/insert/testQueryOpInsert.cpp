#include <pch_test.h>
#include <dummy_array_in_memory.h>

using namespace msdb::core;

TEST(query_op_insert, insert_from_memory_4x4)
{
	msdb::Context ctx;
	auto desc = msdb::dummy::array_mem_char_4x4::getDummyArrayDesc();

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
	{
		const uint64_t dataSize = 16;
		auto pData = std::make_shared<std::array<char, dataSize>>();

		for (int i = 0; i < dataSize; ++i)
		{
			pData->at(i) = i;
		}
		
		std::map<attributeId, msdb::core::opParamMemory> attrs;
		attrs[0] = msdb::core::opParamMemory(pData, dataSize * sizeof(char));

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
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		msdb::printResultArray(ra);

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
}