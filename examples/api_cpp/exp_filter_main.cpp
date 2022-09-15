#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy.h>
#include <dummy_astronomy_array.h>
#include <array/arrayMgr.h>
#include <thread>
#include <windows.h>
#include "exp_filter.h"

using namespace msdb;
using namespace msdb::dummy::data_star1024x1024;
using namespace msdb::experiments::data_star1024x1024;

void exeExperiment(int numTest, encodingType compType)
{
	{
		auto afl = Consume(getArrayBuildAFL(materializedType::FLATTEN, compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(getInsertSaveAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(getLoadAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto result = exeFilterTest(numTest, compType);
	}
}

void exeExperiment(int numTest, encodingType compType, attrIndexType idxType)
{
	{
		auto afl = Consume(getArrayBuildAFL(materializedType::FLATTEN, compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(getBuildIndexAFL(compType, idxType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(getSaveIndexAFL(compType, idxType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(getLoadIndexAFL(compType, idxType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(getInsertSaveAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(getLoadAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto result = exeFilterTest(numTest, compType);
	}
}

int main()
{
	int numTest = 3;

	//exeExperiment(numTest, encodingType::RAW);
	exeExperiment(numTest, encodingType::SEACOW, attrIndexType::MMT);
	//exeExperiment(numTest, encodingType::SEACOW_HUFFMAN, attrIndexType::MMT);
	//exeExperiment(numTest, encodingType::SPIHT);
	//exeExperiment(numTest, encodingType::COMPASS);
	//exeExperiment(numTest, encodingType::HUFFMAN);
	//exeExperiment(numTest, encodingType::LZW_HUFFMAN);
	//exeExperiment(numTest, encodingType::LZW);

	//exeExperiment(numTest, encodingType::COMPASS, attrIndexType::COMPASS);	
	//exeExperiment(numTest, encodingType::ADAPTHUFFMAN);
	//exeExperiment(numTest, encodingType::ZIP);

	return 0;
}