#pragma once
#ifndef _MSDB_HUFFMANBLOCK_H_
#define _MSDB_HUFFMANBLOCK_H_

#include <pch.h>
#include <array/memBlock.h>
#include <compression/adaptiveHuffmanCode.h>

namespace msdb
{
namespace core
{
class huffmanBlock;
using pHuffmanBlock = std::shared_ptr<huffmanBlock>;

class huffmanBlock : public memBlock
{
public:
	huffmanBlock(pBlockDesc desc);
	virtual ~huffmanBlock();

public:
	template<typename Cty_, typename Ty_>
	void serializeTy(aHuffmanCoder<Cty_, Ty_>& coder, bstream& out)
	{
		auto iit = this->getItemIterator();

		while(!iit->isEnd())
		{
			coder.encode(out, (**iit).get<Ty_>());

			++(*iit);
		}
	}

	template<typename Cty_, typename Ty_>
	void deserializeTy(aHuffmanCoder<Cty_, Ty_>& coder, bstream& in)
	{
		auto iit = this->getItemIterator();

		while (!iit->isEnd())
		{
			Ty_ value = coder.decode(in);
			(**iit).set(value);

			++(*iit);
		}
	}
};
}		// core
}		// msdb
#endif	// _MSDB_HUFFMANBLOCK_H_