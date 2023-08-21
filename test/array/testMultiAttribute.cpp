#include "gtest/gtest.h"
#include <dummy/dummy_test_array.h>
#include <system/storageMgr.h>
#include <array/arrayMgr.h>
#include <util/datatype.h>
#include <dummy_array_in_memory.h>

namespace msdb
{
namespace test
{
TEST(multi_attributes, array_uint8_2_16x16_4x4)
{
	//////////////////////////////
	// Build Two Attr 2D array
	EXPECT_EQ(msdb::dummy::array_uint8_2_16x16_4x4::buildArray(), Query::Status::COMPLETE);

	//////////////////////////////
	// Examine Array Description
	{
		auto arr = core::arrayMgr::instance()->getArrayDesc(msdb::dummy::array_uint8_2_16x16_4x4::aid);
		auto attrDesc = arr->getAttrDescs();

		EXPECT_EQ(attrDesc->size(), 2);
		{
			core::attributeId attrId = 0;
			auto attr = attrDesc->at(attrId);

			EXPECT_EQ(attr->getId(), attrId);
			EXPECT_EQ(typeid(attr->getDataType()), typeid(msdb::dummy::array_uint8_2_16x16_4x4::typeAttr_0));
			EXPECT_TRUE(attr->getName() == msdb::dummy::array_uint8_2_16x16_4x4::nameAttr_0);
		}
		{
			core::attributeId attrId = 1;
			auto attr = attrDesc->at(attrId);

			EXPECT_EQ(attr->getId(), attrId);
			EXPECT_EQ(typeid(attr->getDataType()), typeid(msdb::dummy::array_uint8_2_16x16_4x4::typeAttr_1));
			EXPECT_TRUE(attr->getName() == msdb::dummy::array_uint8_2_16x16_4x4::nameAttr_1);
		}
	}
}		// TEST
}		// test
}		// msdb