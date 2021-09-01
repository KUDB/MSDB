#pragma once
#ifndef _MSDB_ZIPCHUNK_H_
#define _MSDB_ZIPCHUNK_H_

#include <pch.h>
#include <array/blockChunk.h>
#include <compression/zipBlock.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
class zipChunk;
using pZipChunk = std::shared_ptr<zipChunk>;

class zipChunk : public memBlockChunk
{
public:
	zipChunk(pChunkDesc desc);
	virtual ~zipChunk();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serializeTy(std::stringstream& compressed)
	{
		//////////////////////////////
		// Block IO
		//auto blockItr = this->getBlockIterator();
		//while (!blockItr->isEnd())
		//{
		//	if (blockItr->isExist())
		//	{
		//		pZipBlock zBlock = std::static_pointer_cast<zipBlock>(**blockItr);
		//		zBlock->serializeTy<Ty_>(compressed);
		//	}

		//	++(*blockItr);
		//}
		//////////////////////////////

		//////////////////////////////
		// Chunk IO
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;

		std::stringstream chunkCompressed;
		std::stringstream origin;
		origin.write((const char*)this->getBuffer()->getReadData(), this->getBuffer()->size());
		out.push(boost::iostreams::zlib_compressor());
		out.push(origin);
		boost::iostreams::copy(out, chunkCompressed);

		size_t mChunkSize = getSize(chunkCompressed);
		compressed << static_cast<size_t>(mChunkSize);

		compressed << chunkCompressed.rdbuf();
		chunkCompressed.clear();
		//////////////////////////////
	}

	template<class Ty_>
	void deserializeTy(std::stringstream& compressed)
	{
		//this->bufferAlloc();

		//////////////////////////////
		// Block IO
		//auto blockItr = this->getBlockIterator();
		//while (!blockItr->isEnd())
		//{
		//	if (blockItr->isExist())
		//	{
		//		pZipBlock zBlock = std::static_pointer_cast<zipBlock>(**blockItr);
		//		zBlock->deserializeTy<Ty_>(compressed);
		//	}

		//	++(*blockItr);
		//}
		//////////////////////////////

		//////////////////////////////
		// Chunk IO
		size_t mChunkSize;
		compressed >> mChunkSize;

		assert(mChunkSize <= this->getBuffer()->size());

		char* tempBuffer = new char[mChunkSize];
		compressed.read(tempBuffer, mChunkSize);

		std::stringstream chunkCompressed;
		chunkCompressed.write(tempBuffer, mChunkSize);

		std::stringstream decompressed;
		boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
		out.push(boost::iostreams::zlib_decompressor());
		out.push(chunkCompressed);
		boost::iostreams::copy(out, decompressed);

		delete[] tempBuffer;

		memcpy(this->getBuffer()->getData(), decompressed.str().c_str(), this->getBuffer()->size());
		//////////////////////////////
	}
};
}		// core
}		// msdb
#endif	// _MSDB_ZIPCHUNK_H_
