#pragma once
#ifndef _MSDB_LZWHUFFMANCHUNK_H_
#define _MSDB_LZWHUFFMANCHUNK_H_

#include <pch.h>
#include <array/blockChunk.h>
#include <io/bitstream.h>
#include <compression/lzwEncoder.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
class lzwHuffmanChunk;
using pLzwHuffmanChunk = std::shared_ptr<lzwHuffmanChunk>;

class lzwHuffmanChunk : public memBlockChunk
{
public:
	lzwHuffmanChunk(pChunkDesc desc);
	virtual ~lzwHuffmanChunk();

	using lzwCodeType = std::uint16_t;

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

	template<typename Ty_>
	void serializeTy(bstream& out)
	{
		bstream lzwOut;
		lzwCoder<lzwCodeType> lzwEncoder;
		lzwEncoder.encode(lzwOut, (const unsigned char*)this->cached_->getReadData(), this->cached_->size());

		out << setw(sizeof(size_type) * CHAR_BIT) << lzwOut.capacity();

		//////////////////////////////
		huffmanCoder<uint16_t, uint8_t> huffmanEncoder;
		huffmanEncoder.encode(out, (uint8_t*)lzwOut.data(), lzwOut.capacity());
	}

	template<class Ty_>
	void deserializeTy(bstream& in)
	{
		size_type lzwSize = 0;
		in >> setw(sizeof(size_type) * CHAR_BIT) >> lzwSize;

		uint8_t* tempBuffer = new uint8_t[lzwSize];

		huffmanCoder<uint16_t, uint8_t> huffmanDecoder;
		huffmanDecoder.decode((uint8_t*)tempBuffer, lzwSize, in);

		bstream lzwIn;
		lzwIn.resize(lzwSize);
		memcpy(lzwIn.data(), tempBuffer, lzwSize);

		delete[] tempBuffer;

		lzwCoder<lzwCodeType> lzwDecoder;
		lzwDecoder.decode((unsigned char*)this->cached_->getData(), this->cached_->size(), lzwIn);
	}

private:
	static const lzwCodeType dms{ std::numeric_limits<lzwCodeType>::max() };
};
}		// core
}		// msdb
#endif	// _MSDB_LZWHUFFMANCHUNK_H_