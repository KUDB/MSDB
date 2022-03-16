#pragma once
#ifndef _MSDB_SEHUFFMANCHUNK_H_
#define _MSDB_SEHUFFMANCHUNK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <io/bitstream.h>
#include <compression/seChunk.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
class seHuffmanChunk;
using pSeHuffmanChunk = std::shared_ptr<seHuffmanChunk>;

class seHuffmanChunk : public seChunk
{
public:
	seHuffmanChunk(pChunkDesc desc);
	virtual ~seHuffmanChunk();

	using lzwCodeType = std::uint16_t;

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serializeTy(bstream& out)
	{
		bstream seOut;
		this->seEncode<Ty_>(seOut);

		out << setw(sizeof(size_type) * CHAR_BIT) << seOut.capacity();

		//////////////////////////////
		huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
		huffmanEncoder.encode(out, (uint8_t*)seOut.data(), seOut.capacity());
	}

	template<class Ty_>
	void deserializeTy(bstream& in)
	{
		size_type seSize = 0;
		in >> setw(sizeof(size_type) * CHAR_BIT) >> seSize;
		uint8_t* tempBuffer = new uint8_t[seSize];

		huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
		huffmanDecoder.decode((uint8_t*)tempBuffer, seSize, in);

		//////////////////////////////
		bstream seIn;
		seIn.resize(seSize);
		memcpy(seIn.data(), tempBuffer, seSize);
		delete[] tempBuffer;

		this->seDecode<Ty_>(seIn);
	}
};
}		// core
}		// msdb
#endif	// _MSDB_SEHUFFMANCHUNK_H_