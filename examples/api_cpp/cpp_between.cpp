#include <iostream>
#include <api_cpp/cpp_context.h>
#include <api_cpp/msdb.h>
#include <dummy/dummy.h>

int main()
{
	msdb::Context ctx;
	auto afl = msdb::Between(
		msdb::Load(
			msdb::Array(ctx, DUMMY_PATH(star_1024x1024))),
		msdb::Domain(msdb::Coordinate({ 0, 0 }), msdb::Coordinate({ 128, 128 }))
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

	for(int i = 0; i < dimBuffer->size(); ++i)
	{
		std::cout << "[" << dimBuffer->at(i).toString() << "]: " << attr_01_buffer->at(i);
	}

	ra.close();

	return 0;
}