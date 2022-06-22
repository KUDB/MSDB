#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy.h>
#include <array/arrayMgr.h>

int main()
{
	auto arrDesc = msdb::dummy::data_star1024x1024::getDummyArrayDesc();
	msdb::core::arrayMgr::instance()->registArray(arrDesc);

	msdb::Context ctx;
	auto afl = msdb::Insert(
		msdb::Array(ctx, msdb::dummy::data_star1024x1024::arrName),
		msdb::dummy::data_star1024x1024::filePath);

	auto qry = msdb::Query(afl);


	return 0;
}