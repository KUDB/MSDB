#pragma once
#ifndef _MSDB_HUFFMANCHUNK_H_
#define _MSDB_HUFFMANCHUNK_H_

#include <pch.h>
#include <array/blockChunk.h>
#include <compression/huffmanBlock.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
class huffmanChunk;
using pHuffmanChunk = std::shared_ptr<huffmanChunk>;

class huffmanChunk : public memBlockChunk
{
public:
	huffmanChunk(pChunkDesc desc);
	virtual ~huffmanChunk();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

private:
	template<typename Cty_, typename Ty_>
	void serializeTy(bstream& out)
	{
		huffmanCoderLonger<uint32_t, uint8_t> huffmanEncoder;
		huffmanEncoder.encode(out, (uint8_t*)this->getBuffer()->getReadData(), this->getBuffer()->size());
	}

	template<typename Cty_, typename Ty_>
	void deserializeTy(bstream& in)
	{
		huffmanCoderLonger<uint32_t, uint8_t> huffmanDecoder;
		huffmanDecoder.decode((uint8_t*)this->getBuffer()->getData(), this->getBuffer()->size(), in);
	}
};
}		// core
}		// msdb
#endif	// _MSDB_HUFFMANCHUNK_H_