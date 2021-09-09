#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy/dummy.h>
#include <array/arrayMgr.h>

int main()
{
	auto arrDesc = msdb::dummy::star1024x1024::getDummyArrayDesc();
	msdb::core::arrayMgr::instance()->registArray(arrDesc);

	msdb::Context ctx;

	auto afl = msdb::Between(
		msdb::Filter(
			msdb::Insert(
				msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
				msdb::dummy::star1024x1024::filePath
			),
			msdb::Attribute("ATTR_1") == 4
		),
		msdb::Domain(msdb::Coordinate({ 128 - 2, 128 - 2 }), msdb::Coordinate({ 128 + 2, 128 + 2 }))
	);

	//auto afl = msdb::Filter(
	//	msdb::Insert(
	//		msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//		msdb::dummy::star1024x1024::filePath
	//	),
	//	msdb::Attribute("ATTR_1") == 4
	//);

	//auto afl = msdb::Between(
	//	msdb::Insert(
	//		msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//		msdb::dummy::star1024x1024::filePath
	//	),
	//	msdb::Domain(msdb::Coordinate({ 128-2, 128-2 }), msdb::Coordinate({ 128 + 2, 128 + 2 }))
	//);

	//auto afl =
	//	msdb::Insert(
	//		msdb::Array(ctx, msdb::dummy::star1024x1024::arrName),
	//		msdb::dummy::star1024x1024::filePath
	//	); 

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

	return 0;
}