#include <iostream>
#include <api_cpp/cpp_context.h>
#include <api_cpp/msdb.h>
#include <dummy.h>

int main()
{
	auto arrDesc = msdb::dummy::data_star1024x1024::getDummyArrayDesc();
	msdb::core::arrayMgr::instance()->registArray(arrDesc);

	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::Load(msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName));

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	if (qry.getStatus() == msdb::Query::Status::FAIL)
	//	{
	//		// Query failed
	//		return 0;
	//	}
	//	auto dimBuffer = ra.getDimBuffer();
	//	auto attr_01_buffer = ra.getAttrBuffer<uint8_t>(0);

	//	for (int i = 0; i < dimBuffer->size(); ++i)
	//	{
	//		std::cout << dimBuffer->at(i).toString() << ": " << static_cast<int>(attr_01_buffer->at(i)) << std::endl;
	//	}

	//	ra.close();
	//}

	{
		msdb::Context ctx;
		auto afl = msdb::Between(
			msdb::Load(
				msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName)),
			msdb::Domain(msdb::Coordinates({ 50, 50 }), msdb::Coordinates({ 60, 60 }))
		);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		if (qry.getStatus() == msdb::Query::Status::FAIL)
		{
			// Query failed
			return 0;
		}
		auto dimBuffer = ra.getDimBuffer();
		auto attr_01_buffer = ra.getAttrBuffer<uint8_t>(0);

		for (int i = 0; i < dimBuffer->size(); ++i)
		{
			std::cout << dimBuffer->at(i).toString() << ": " << static_cast<int>(attr_01_buffer->at(i)) << std::endl;
		}

		ra.close();
	}

	return 0;
}