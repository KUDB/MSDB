#pragma once
#ifndef _MSDB_ADAPTHUFFMANCHUNK_H_
#define _MSDB_ADAPTHUFFMANCHUNK_H_

#include <pch.h>
#include <array/blockChunk.h>
#include <compression/huffmanBlock.h>
#include <compression/adaptiveHuffmanCode.h>
#include <compression/huffmanCode.h>

namespace msdb
{
namespace core
{
class adaptHuffmanChunk;
using pAdaptHuffmanChunk = std::shared_ptr<adaptHuffmanChunk>;

class adaptHuffmanChunk : public memBlockChunk
{
public:
	adaptHuffmanChunk(pChunkDesc desc);
	virtual ~adaptHuffmanChunk();

public:
	virtual pBlock makeBlock(const blockId bId) override;

public:
	virtual void serialize(std::ostream& os) override;
	virtual void deserialize(std::istream& is) override;

private:
	template<typename Cty_, typename Ty_>
	void serializeTy(bstream& out)
	{
		//////////////////////////////
		// Block based adaptive huffman coding
		//aHuffmanCoder<Cty_, Ty_> coder(sizeof(Ty_) * CHAR_BIT);
		//auto bit = this->getBlockIterator();
		//while(!bit->isEnd())
		//{
		//	if(bit->isExist())
		//	{
		//		pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
		//		hb->serializeTy<Cty_, Ty_>(coder, out);
		//	}
		//	++(*bit);
		//}

		//////////////////////////////
		// Adaptive huffman coding
		aHuffmanCoder<uint16_t, uint8_t> coder(sizeof(uint8_t) * CHAR_BIT);
		auto buffer = (const char*)this->getBuffer()->getReadData();
		auto size = this->getBuffer()->size();
		for (int i = 0; i < size; ++i)
		{
			coder.encode(out, buffer[i]);
		}

		//////////////////////////////
	}

	template<typename Cty_, typename Ty_>
	void deserializeTy(bstream& in)
	{
		//aHuffmanCoder<Cty_, Ty_> coder(sizeof(Ty_) * CHAR_BIT);
		//auto bit = this->getBlockIterator();
		//while (!bit->isEnd())
		//{
		//	if (bit->isExist())
		//	{
		//		auto a = *bit;
		//		pHuffmanBlock hb = std::static_pointer_cast<huffmanBlock>(**bit);
		//		hb->deserializeTy<Cty_, Ty_>(coder, in);
		//	}
		//	++(*bit);
		//}

		//////////////////////////////
		// Adaptive huffman coding
		aHuffmanCoder<uint16_t, uint8_t> coder(sizeof(uint8_t) * CHAR_BIT);
		auto buffer = (unsigned char*)this->getBuffer()->getData();
		auto size = this->getBuffer()->size();
		for (int i = 0; i < size; ++i)
		{
			unsigned char value = coder.decode(in);
			buffer[i] = value;
		}
	}
};
}		// core
}		// msdb
#endif	// _MSDB_ADAPTHUFFMANCHUNK_H_