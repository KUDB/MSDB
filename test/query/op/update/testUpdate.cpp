#include "gtest/gtest.h"

#include <dummy_small_array.h>
#include <dummy_query_util.h>
#include <op/update/update_plan.h>
#include <api_cpp_operator/cpp_io_operators.h>
#include <io/io_util.h>
#include <filesystem>

using std::filesystem::directory_iterator;

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

TEST(fast_update, fast_update_1x32x32x3)
{
	msdb::Context ctx;
	core::arrayId aid = 103203203;
	core::attributeId attrId = 0;
	msdb::core::arrayMgr::instance()->removeArray(aid);

	std::string basePath = "E:/Datasets/cifar/test/";
	std::vector<std::string> filePath;
	for (const auto& file : directory_iterator(basePath))
	{
		filePath.push_back(file.path().string());
	}
	
	dummy::array_1x32x32x3 arr(std::string("cifa_10_1x32x32x3"), aid, { (int)filePath.size(), 32, 32, 3}, {});

	{
		BOOST_LOG_TRIVIAL(debug) << "==========BUILD==========";
		auto buildAfl = arr.getArrayBuildAFL();
		auto qry = msdb::Query(buildAfl);
		auto ra = qry.execute();
		EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);
		ra.close();
	}
	
	{
		BOOST_LOG_TRIVIAL(debug) << "==========LOAD==========";
		auto loadAfl = msdb::Load(msdb::Array(ctx, aid));
		auto qry = msdb::Query(loadAfl);
		qry.setRawResultOut();

		auto ra = qry.execute();
		EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);
		EXPECT_TRUE(ra.isRawResultOut());
		auto outArr = ra.getRawResult();
		outArr->print();
		ra.close();
	}

	bool updateOnly = false;
	//for (int fid = 0; fid < filePath.size(); ++fid)
	for (int fid = 0; fid < 5; ++fid)
	{
		{
			BOOST_LOG_TRIVIAL(debug) << "==========FAST UPDATE==========";
			auto updateAfl = msdb::Save(
				msdb::FastUpdate(
					msdb::Array(ctx, aid),
					{ {attrId, filePath[fid]} }, msdb::Coordinates({ fid, 0, 0, 0 }), msdb::Coordinates({ fid+1, 32, 32, 3 })));
			auto qry = msdb::Query(updateAfl);

			if(!updateOnly)	qry.setRawResultOut();

			auto ra = qry.execute();

			if (!updateOnly)
			{
				EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);
				EXPECT_TRUE(ra.isRawResultOut());
				auto outArr = ra.getRawResult();
				outArr->print();
			}
			ra.close();
		}

		if (!updateOnly)
		{
			BOOST_LOG_TRIVIAL(debug) << "==========LOAD==========";
			auto loadAfl = msdb::Load(msdb::Array(ctx, aid));
			auto qry = msdb::Query(loadAfl);
			qry.setRawResultOut();

			auto ra = qry.execute();
			EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);
			EXPECT_TRUE(ra.isRawResultOut());
			auto outArr = ra.getRawResult();
			outArr->print();
		}

		if(!updateOnly)
		{
			BOOST_LOG_TRIVIAL(debug) << "==========LOAD==========";
			auto loadAfl = msdb::Between(msdb::Load(msdb::Array(ctx, aid)), msdb::Domain(msdb::core::coordinates({fid, 0, 0, 0}), msdb::core::coordinates({fid+1, 32, 32, 3})));
			//auto loadAfl = msdb::Load(msdb::Array(ctx, aid));
			auto qry = msdb::Query(loadAfl);
			qry.setRawResultOut();

			auto ra = qry.execute();
			EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);
			EXPECT_TRUE(ra.isRawResultOut());
			auto outArr = ra.getRawResult();
			outArr->print();


			BOOST_LOG_TRIVIAL(debug) << "==========COMPARISON==========";
			//auto attrBuffer = ra.getAttrBuffer<uint8_t>(0);
			auto cit = outArr->getChunkIterator(attrId);
			auto chunkCoor = outArr->itemCoorToChunkCoor({ fid, 0, 0, 0 });
			cit->moveTo(chunkCoor);

			EXPECT_FALSE(cit->isEnd());
			EXPECT_TRUE(cit->isExist());

			auto bit = (*cit)->getBlockIterator();
			EXPECT_FALSE(bit->isEnd());
			EXPECT_TRUE(bit->isExist());

			auto iit = (*bit)->getItemIterator();

			try
			{
				BOOST_LOG_TRIVIAL(debug) << "==========IMAGE READ==========";
				size_t bytes, length;
				void* raw_data;
				msdb::core::readImageFile(filePath[fid], &raw_data, bytes, length);

				EXPECT_EQ(iit->getCapacity(), length);

				uint8_t* img_data = (uint8_t*)raw_data;

				int i = 0;
				while (!iit->isEnd())
				{
					auto arr_v = (**iit).getUint8();
					
					EXPECT_EQ(arr_v, img_data[i++]);

					++(*iit);
				}

				delete[] raw_data;
			}
			catch (...)
			{
				EXPECT_TRUE(false);
			}
			ra.close();
		}
	}

	core::arrayMgr::instance()->saveAllArrayDesc();
}
}		// test
}		// msdb