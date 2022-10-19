#include <pch_test.h>
#include <dummy/dummy_test_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <util/datatype.h>
#include <dummy_array_in_memory.h>

namespace msdb
{
namespace test
{
TEST(multiAttribute, twoAttr2D)
{
	//////////////////////////////
	// Build Two Attr 2D array
	{
		auto buildAfl = msdb::dummy::array_mem_twoattr_2d::getArrayBuildAFL();
		auto qry = msdb::Query(buildAfl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		//std::cout << buildQry.getTimer()->getDetailResult() << std::endl;

		EXPECT_TRUE(qry.getStatus() == msdb::Query::Status::COMPLETE);
	}
	//////////////////////////////
	// Examine Array Description
	{
		auto arr = core::arrayMgr::instance()->getArrayDesc(msdb::dummy::array_mem_twoattr_2d::aid);
		auto attrDesc = arr->getAttrDescs();

		EXPECT_EQ(attrDesc->size(), 2);
		{
			core::attributeId attrId = 0;
			auto attr = attrDesc->at(attrId);

			EXPECT_EQ(attr->getId(), attrId);
			EXPECT_EQ(typeid(attr->getDataType()), typeid(msdb::dummy::array_mem_twoattr_2d::typeAttr_0));
			EXPECT_TRUE(attr->getName() == msdb::dummy::array_mem_twoattr_2d::nameAttr_0);
		}
		{
			core::attributeId attrId = 1;
			auto attr = attrDesc->at(attrId);

			EXPECT_EQ(attr->getId(), attrId);
			EXPECT_EQ(typeid(attr->getDataType()), typeid(msdb::dummy::array_mem_twoattr_2d::typeAttr_1));
			EXPECT_TRUE(attr->getName() == msdb::dummy::array_mem_twoattr_2d::nameAttr_1);
		}
	}
}		// TEST
}		// test
}		// msdb