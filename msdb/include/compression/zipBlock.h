#pragma once
#ifndef _MSDB_ZIPBLOCK_H_
#define _MSDB_ZIPBLOCK_H_

#include <pch.h>
#include <array/memBlock.h>
#include <util/ioutil.h>

namespace msdb
{
namespace core
{
class zipBlock;
using pZipBlock = std::shared_ptr<zipBlock>;

class zipBlock : public memBlock
{
public:
	zipBlock(pBlockDesc desc);
	virtual ~zipBlock();

public:
	template<typename Ty_>
	void serializeTy(std::stringstream& compressed)
	{
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;

		std::stringstream blockCompressed;
		std::stringstream origin;
		origin.write((const char*)this->getBuffer()->getReadData(), this->getBuffer()->size());
		out.push(boost::iostreams::zlib_compressor());
		out.push(origin);
		boost::iostreams::copy(out, blockCompressed);

		size_t mBlockSize = getSize(blockCompressed);
		compressed << static_cast<size_t>(mBlockSize);

		compressed << blockCompressed.rdbuf();
	}

	template<typename Ty_>
	void deserializeTy(std::stringstream& compressed)
	{
		size_t mBlockSize;
		compressed >> mBlockSize;

		assert(mBlockSize <= this->getBuffer()->size() * 2);

		char* tempBuffer = new char[mBlockSize];
		compressed.read(tempBuffer, mBlockSize);

		std::stringstream blockCompressed;
		blockCompressed.write(tempBuffer, mBlockSize);
		
		std::stringstream decompressed;
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		out.push(boost::iostreams::zlib_decompressor());
		out.push(blockCompressed);
		boost::iostreams::copy(out, decompressed);

		delete[] tempBuffer;

		memcpy(this->getBuffer()->getData(), decompressed.str().c_str(), this->getBuffer()->size());
	}
};
}		// core
}		// msdb
#endif	// _MSDB_ZIPBLOCK_H_