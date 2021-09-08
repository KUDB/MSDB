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
			msdb::Dimension("Y", 0, 1024, 128, 32),
			msdb::Dimension("X", 0, 1024, 128, 32) 
		}, 
		{
			msdb::Attribute("ATTR_1", msdb::core::eleType::CHAR)
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