#include <pch_test.h>
#include <dummy_array_in_memory.h>
#include <op/load/load_plan.h>
#include <op/project/project_plan.h>

namespace msdb
{
using namespace core;

TEST(query_op_project, project_two_attr)
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
	{
		const uint64_t dataSize = 16 * 16;
		auto pDataA1 = std::make_shared<std::array<uint8_t, dataSize>>();
		auto pDataA2 = std::make_shared<std::array<uint8_t, dataSize>>();

		for (int i = 0; i < dataSize; ++i)
		{
			pDataA1->at(i) = i;
			pDataA2->at(i) = i * 2;
		}

		std::map<attributeId, msdb::core::opParamMemory> attrs;
		attrs[0] = msdb::core::opParamMemory(pDataA1, dataSize * sizeof(uint8_t));
		attrs[1] = msdb::core::opParamMemory(pDataA2, dataSize * sizeof(uint8_t));

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
		qry.setVerbose();
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
	//////////////////////////////////////////////////
	{

		auto loadPlan = std::make_shared<core::load_plan>();
		{
			core::parameters params = {
				std::make_shared<core::opParamArray>(desc)
			};
			loadPlan->setParamSet(std::make_shared<core::load_array_pset>(params));
		}

		auto projectPlan = std::make_shared<op::project_plan>();
		{
			//std::vector<std::string> attrList = { msdb::dummy::array_mem_twoattr_2d::nameAttr_1 };
			auto pAttrList = std::make_shared<std::vector<std::string>>();
			pAttrList->push_back(msdb::dummy::array_mem_twoattr_2d::nameAttr_1);
			core::parameters params = {
				std::make_shared<core::opParamPlan>(loadPlan),
				std::make_shared<core::opParamStringList>(pAttrList)
			};
			projectPlan->setParamSet(std::make_shared<op::project_pset>(params));
		}

		auto qry = std::make_shared<core::query>(projectPlan);
		if (qry->submit().isOk())
		{
			EXPECT_TRUE(true);
		}
		else
		{
			EXPECT_TRUE(false);
		}
	}
	//////////////////////////////////////////////////
	{

		auto projectAFL = msdb::op::Project(
			msdb::Array(ctx, msdb::dummy::array_mem_twoattr_2d::arrName),
			std::vector<std::string>({ msdb::dummy::array_mem_twoattr_2d::nameAttr_1 })
		);

		std::cout << "=====" << std::endl;
		std::cout << projectAFL->toString(0) << std::endl;
		std::cout << "=====" << std::endl;

		auto qry = msdb::Query(projectAFL);
		qry.setVerbose();
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
}
}		// msdb