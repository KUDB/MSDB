#include <pch_experiments.h>

#include <filesystem>

#include <dummy_small_array.h>
#include <dummy_query_util.h>

#include <io/io_util.h>
#include <op/update/update_plan.h>
#include <op/insert/insert_plan.h>
#include <api_cpp_operator/cpp_io_operators.h>

using std::filesystem::directory_iterator;

namespace msdb
{
namespace experiments
{
TEST(ml_04_save, cifar10_1x1_test_label_insert)
{
	msdb::Context ctx;
	core::arrayId aid = 103203201;
	core::attributeId attrId = 0;
	msdb::core::arrayMgr::instance()->removeArray(aid);
	std::string basePath = "E:/Datasets/cifar10/test/";
	std::vector<std::string> fileList;
	for (const auto& file : directory_iterator(basePath))
	{
		fileList.push_back(file.path().string());
	}

	core::dimension dim = { (int)fileList.size(),1 };
	dummy::array_1x1 arr(std::string("cifar10_test_label"), aid, dim, dim, dim, {});
	{
		BOOST_LOG_TRIVIAL(debug) << "==========BUILD==========";
		auto buildAfl = arr.getArrayBuildAFL();
		auto qry = msdb::Query(buildAfl);
		auto ra = qry.execute();
		EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);
		ra.close();
	}

	std::string insertFilePath = "E:/Datasets/cifar10/test_label.bin";
	{
		BOOST_LOG_TRIVIAL(debug) << "==========INSERT==========";
		auto updateAfl = msdb::Save(
			msdb::Insert(
				msdb::Array(ctx, aid), insertFilePath));
		auto qry = msdb::Query(updateAfl);

		auto ra = qry.execute();
		EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);

		ra.close();
	}

	bool verify = true;
	if (verify)
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

		std::ifstream fin;
		fin.open(insertFilePath, std::ios::in | std::ios::binary);
		EXPECT_TRUE(fin.is_open());

		if (fin.is_open())
		{
			fin.seekg(0, fin.end);
			auto fileSize = fin.tellg();
			EXPECT_TRUE(fileSize > 0);
			
			unsigned char* data = new unsigned char[fileSize];
			fin.seekg(0, std::ios::beg);
			fin.read((char*)data, fileSize);
			fin.close();

			EXPECT_TRUE(outArr->isEqual(0, data, fileSize));
		}
	}

	core::arrayMgr::instance()->saveAllArrayDesc();
}
TEST(ml_04_save, cifar10_1x32x32x3_test_img_insert)
{
	msdb::Context ctx;
	core::arrayId aid = 103203203;
	core::attributeId attrId = 0;
	msdb::core::arrayMgr::instance()->removeArray(aid);
	std::string basePath = "E:/Datasets/cifar10/test/";
	std::vector<std::string> filePath;
	for (const auto& file : directory_iterator(basePath))
	{
		filePath.push_back(file.path().string());
	}

	dummy::array_1x32x32x3 arr(std::string("cifar10_test_image_32x32x3"), aid, { (int)filePath.size(), 32, 32, 3 }, {});
	{
		BOOST_LOG_TRIVIAL(debug) << "==========BUILD==========";
		auto buildAfl = arr.getArrayBuildAFL();
		auto qry = msdb::Query(buildAfl);
		auto ra = qry.execute();
		EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);
		ra.close();
	}

	//////////////////////////////
	// Set False to verify insertion query result.
	bool verify = false;
	//////////////////////////////

	for (int fid = 0; fid < filePath.size(); ++fid)
	{
		{
			BOOST_LOG_TRIVIAL(debug) << "==========FAST UPDATE==========";
			auto updateAfl = msdb::Save(
				msdb::FastUpdate(
					msdb::Array(ctx, aid),
					{ {attrId, filePath[fid]} }, msdb::Coordinates({ fid, 0, 0, 0 }), msdb::Coordinates({ fid + 1, 32, 32, 3 })));
			auto qry = msdb::Query(updateAfl);

			if (verify)	qry.setRawResultOut();

			auto ra = qry.execute();
			EXPECT_EQ(qry.getStatus(), msdb::Query::Status::COMPLETE);

			if (verify)
			{
				EXPECT_TRUE(ra.isRawResultOut());
				auto outArr = ra.getRawResult();
				outArr->print();
			}
			ra.close();
		}

		if (verify)
		{
			BOOST_LOG_TRIVIAL(debug) << "==========LOAD==========";
			auto loadAfl = msdb::Between(msdb::Load(msdb::Array(ctx, aid)), msdb::Domain(msdb::core::coordinates({ fid, 0, 0, 0 }), msdb::core::coordinates({ fid + 1, 32, 32, 3 })));
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
}		// TEST
}		// experiments
}		// msdb