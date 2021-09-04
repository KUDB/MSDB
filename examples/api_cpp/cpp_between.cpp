#include <iostream>
#include <api_cpp/cpp_context.h>
#include <api_cpp/msdb.h>
#include <dummy/dummy.h>

int main()
{
	msdb::Context ctx;
	auto qry = msdb::Between(
		msdb::Load(
			msdb::Array(ctx, DUMMY_PATH(star_1024x1024))),
		msdb::Domain(msdb::Coordinate({ 0, 0 }), msdb::Coordinate({ 128, 128 }))
	);

	auto ra = qry.execute();
	if (qry.getStatus() == msdb::Query::Status::FAIL)
	{
		// Query failed
		return 0;
	}

	ra.close();

	return 0;
}