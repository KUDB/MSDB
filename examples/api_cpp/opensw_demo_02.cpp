#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <array/arrayMgr.h>
#include <thread>
#include <windows.h>

#include <api_cpp_operator/cpp_io_operators.h>

#include <exeQuery/exe_build_astronomy_array.h>
#include <exeQuery/exe_build_astronomy_array_3d.h>
#include <exeQuery/exe_save_astronomy_array.h>

namespace msdb
{
namespace experiments
{

void demo()
{
	char input;

	Sleep(5000);

	//////////////////////////////
	// data_mercury20480x10240
	{
		msdb::Context ctx;
		auto afl = msdb::dummy::data_mercury20480x10240::getArrayBuildAFL(core::materializedType::FLATTEN, core::encodingType::NONE);
		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;
		Sleep(1000);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;
		Sleep(2000);
	}

	{
		msdb::Context ctx;
		auto afl = msdb::Consume(msdb::dummy::getInsertSaveAFL(
			msdb::dummy::data_mercury20480x10240::arrName, msdb::dummy::data_mercury20480x10240::filePath, core::encodingType::NONE));
		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;
		Sleep(1000);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;
		Sleep(1000);
		system("cls");
		Sleep(1000);
	}

	//////////////////////////////
	// Range query
	{
		msdb::Context ctx;
		//auto afl = msdb::Between(
		//		msdb::Load(
		//			msdb::Array(ctx, msdb::dummy::data_mercury20480x10240::arrName)
		//		),
		//		msdb::Domain(msdb::Coordinates({ 6500, 14500}), msdb::Coordinates({ 6600, 14600 }))
		//);

		auto afl = msdb::Between(
			msdb::Load(
				msdb::Array(ctx, msdb::dummy::data_mercury20480x10240::arrName)
			),
			msdb::Domain({ 7500, 14500 }, { 7600, 14600 })
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;
		Sleep(1000);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;
		Sleep(1000);
		system("cls");
		Sleep(1000);
	}

	//////////////////////////////
	// Range Filter Query
	{
		msdb::Context ctx;
		auto afl = msdb::Filter(
			msdb::Between(
				msdb::Load(
					msdb::Array(ctx, msdb::dummy::data_mercury20480x10240::arrName)
				),
				msdb::Domain(msdb::Coordinates({ 6000, 14000 }), msdb::Coordinates({ 7000, 15000 }))
			),
			msdb::Attribute("ATTR_1") < 10
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;
		Sleep(1000);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;
		Sleep(1000);
		system("cls");
		Sleep(1000);
	}

	//////////////////////////////
	// Another Range Filter Query
	{
		msdb::Context ctx;
		auto afl = msdb::Filter(
			msdb::Between(
				msdb::Load(
					msdb::Array(ctx, msdb::dummy::data_mercury20480x10240::arrName)
				),
				msdb::Domain(msdb::Coordinates({ 4800, 10000}), msdb::Coordinates({ 5200, 10500}))
			),
			msdb::Attribute("ATTR_1") < 10
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;
		Sleep(1000);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;
		Sleep(1000);
		system("cls");
		Sleep(1000);
	}

	{
		msdb::Context ctx;
		auto afl = msdb::Filter(
			msdb::Between(
				msdb::Load(
					msdb::Array(ctx, msdb::dummy::data_mercury20480x10240::arrName)
				),
				msdb::Domain(msdb::Coordinates({ 4800, 10000 }), msdb::Coordinates({ 5200, 10500 }))
			),
			msdb::Attribute("ATTR_1") < 40
		);

		std::cout << "=====" << std::endl;
		std::cout << afl->toString(0) << std::endl;
		std::cout << "=====" << std::endl;
		Sleep(1000);

		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		msdb::printResultArray(ra);
		std::cout << qry.strStatus() << std::endl;
		//std::cout << qry.getTimer()->getDetailResult() << std::endl;
		Sleep(1000);
		system("cls");
		Sleep(1000);
	}


	//////////////////////////////
	// data_nexrad_201711_64x24x1024x2048
	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::dummy::data_nexrad_201711_64x24x1024x2048::getArrayBuildAFL(core::materializedType::FLATTEN, core::encodingType::NONE);
	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;
	//	Sleep(1000);

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);
	//	std::cout << qry.strStatus() << std::endl;
	//	Sleep(2000);
	//}

	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::Consume(msdb::dummy::getInsertSaveAFL(
	//		msdb::dummy::data_nexrad_201711_64x24x1024x2048::arrName, msdb::dummy::data_nexrad_201711_64x24x1024x2048::filePath, core::encodingType::NONE));
	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;
	//	Sleep(1000);

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);
	//	std::cout << qry.strStatus() << std::endl;
	//	//std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//	Sleep(1000);
	//	system("cls");
	//	Sleep(1000);
	//}


	////////////////////////////
	// comp:lzw:data_mercury20480x10240
	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::dummy::data_mercury20480x10240::getArrayBuildAFL(core::materializedType::FLATTEN, core::encodingType::LZW);
	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;
	//	Sleep(1000);

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);
	//	std::cout << qry.strStatus() << std::endl;
	//	//std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//	Sleep(2000);
	//}

	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::Consume(msdb::dummy::getInsertSaveAFL(
	//		msdb::dummy::data_mercury20480x10240::arrName, msdb::dummy::data_mercury20480x10240::filePath, core::encodingType::LZW));
	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;
	//	Sleep(1000);

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);
	//	std::cout << qry.strStatus() << std::endl;
	//	//std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//	Sleep(1000);
	//	system("cls");
	//	Sleep(1000);
	//}


	//////////////////////////////
	//// comp:huffman:data_mercury20480x10240
	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::dummy::data_mercury20480x10240::getArrayBuildAFL(core::materializedType::FLATTEN, core::encodingType::HUFFMAN);
	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;
	//	Sleep(1000);

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);
	//	std::cout << qry.strStatus() << std::endl;
	//	//std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//	Sleep(2000);
	//}

	//{
	//	msdb::Context ctx;
	//	auto afl = msdb::Consume(msdb::dummy::getInsertSaveAFL(
	//		msdb::dummy::data_mercury20480x10240::arrName, msdb::dummy::data_mercury20480x10240::filePath, core::encodingType::HUFFMAN));
	//	std::cout << "=====" << std::endl;
	//	std::cout << afl->toString(0) << std::endl;
	//	std::cout << "=====" << std::endl;
	//	Sleep(1000);

	//	auto qry = msdb::Query(afl);
	//	auto ra = qry.execute();
	//	msdb::printResultArray(ra);
	//	std::cout << qry.strStatus() << std::endl;
	//	std::cout << qry.getTimer()->getDetailResult() << std::endl;
	//	Sleep(1000);
	//	system("cls");
	//	Sleep(1000);
	//}
}	// demo()
}	// experiments
}	// msdb

int main()
{
	msdb::experiments::demo();
	return 0;
}