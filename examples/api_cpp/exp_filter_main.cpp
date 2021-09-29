#include <iostream>
#include <api_cpp/msdb.h>
#include <dummy.h>
#include <array/arrayMgr.h>
#include <thread>
#include <windows.h>
#include "exp_filter.h"

using namespace msdb;

void exeExperiment(int numTest, compressionType compType)
{
	{
		auto afl = Consume(dummy::data_star1024x1024::getArrayBuildAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(dummy::data_star1024x1024::getInsertAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(dummy::data_star1024x1024::getLoadAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto result = experiments::data_star1024x1024::exeFilterTest(numTest, compType);
	}
}

void exeExperiment(int numTest, compressionType compType, attrIndexType idxType)
{
	{
		auto afl = Consume(dummy::data_star1024x1024::getArrayBuildAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(dummy::data_star1024x1024::getBuildIndexAFL(compType, idxType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(dummy::data_star1024x1024::getSaveIndexAFL(compType, idxType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(dummy::data_star1024x1024::getLoadIndexAFL(compType, idxType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(dummy::data_star1024x1024::getInsertAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto afl = Consume(dummy::data_star1024x1024::getLoadAFL(compType));
		std::cout << afl->toString(0) << std::endl;
		auto qry = msdb::Query(afl);
		auto ra = qry.execute();
		std::cout << qry.strStatus() << std::endl;
	}
	{
		auto result = experiments::data_star1024x1024::exeFilterTest(numTest, compType);
	}
}

int main()
{
	int numTest = 3;

	//exeExperiment(numTest, compressionType::RAW);
	//exeExperiment(numTest, compressionType::SEACOW, attrIndexType::MMT);
	//exeExperiment(numTest, compressionType::SEACOW_HUFFMAN, attrIndexType::MMT);
	exeExperiment(numTest, compressionType::SPIHT);
	//exeExperiment(numTest, compressionType::COMPASS, attrIndexType::COMPASS);	
	//exeExperiment(numTest, compressionType::HUFFMAN);
	//exeExperiment(numTest, compressionType::ADAPTHUFFMAN);
	//exeExperiment(numTest, compressionType::LZW_HUFFMAN);
	//exeExperiment(numTest, compressionType::LZW);
	//exeExperiment(numTest, compressionType::ZIP);

	return 0;
}