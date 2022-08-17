#include <pch_test.h>
#include <SZ3/sz3lib.hpp>

namespace msdb
{
namespace test
{
TEST(sz3_encode, sz3lib)
{
	char rawData[4096];
	for (int i = 0; i < 4096; ++i)
	{
		rawData[i] = i;
	}

	//std::string stren = "T:\\" + std::to_string(inChunk->getId()) + ".enout";
	//std::string strde = "T:\\" + std::to_string(inChunk->getId()) + ".deout";
	//std::string strraw = "T:\\" + std::to_string(inChunk->getId()) + ".raw";
	////BOOST_LOG_TRIVIAL(debug) << "szmain start: " << inChunk->getId();
	//const char* argv[] = { "", "-I", "32", "-i", "T:\\0.chunk", "-z", stren.c_str(), "-2", "64", "16", "-M", "ABS", "0", NULL };
	////sz3::szmain(13, argv);
	////BOOST_LOG_TRIVIAL(debug) << "szmain end: " << inChunk->getId();

	//char* out = nullptr;
	////size_t outBytes = sz3::sz3lib(13, argv, (char*)inChunk->getBuffer()->getData(), &out, inChunk->getBuffer()->size());
	//size_t outBytes = sz3::sz3lib(13, argv, rawData, &out, inChunk->getBuffer()->size());

	//const char* argv2[] = { "", "-I", "32", "-z", "T:\\0.chunk", "-o", stren.c_str(), NULL };
	//char* decodeOut = nullptr;
	//sz3::sz3lib(7, argv2, out, &decodeOut, outBytes);

	//std::ofstream fenout(stren.c_str(), std::ios::binary);
	//fenout.write(reinterpret_cast<const char*>(out), outBytes);
	//fenout.close();

	//std::ofstream fdeout(strde.c_str(), std::ios::binary);
	//fdeout.write(reinterpret_cast<const char*>(decodeOut), inChunk->getBuffer()->size());
	//fdeout.close();

	//std::ofstream frawout(strraw.c_str(), std::ios::binary);
	//frawout.write(reinterpret_cast<const char*>(rawData), inChunk->getBuffer()->size());
	//frawout.close();

	//delete out;
	//delete decodeOut;
}
}		// test
}		// msdb