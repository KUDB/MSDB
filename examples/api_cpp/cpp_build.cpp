#include <iostream>
#include <api_cpp/cpp_context.h>
#include <api_cpp/msdb.h>
#include <dummy/dummy.h>

int main()
{
	msdb::Context ctx;
	auto afl = msdb::Build(
		0, "test", 
		{ 
			msdb::DefDimension("Y", 0, 1024, 128, 32),
			msdb::DefDimension("X", 0, 1024, 128, 32) 
		}, 
		{
			msdb::DefAttribute("ATTR_1", msdb::core::eleType::CHAR)
		}
	);

	auto qry = msdb::Query(afl);
	auto ra = qry.execute();
	if (qry.getStatus() == msdb::Query::Status::FAIL)
	{
		// Query failed
		return 0;
	}

	ra.close();
	
	return 0;
}