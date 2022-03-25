#pragma once
#ifndef _MSDB_ADAPTHUFFMANBLOCK_H_
#define _MSDB_ADAPTHUFFMANBLOCK_H_

#include <pch.h>
#include <array/flattenChunk.h>
#include <compression/adaptiveHuffmanCode.h>

namespace msdb
{
namespace core
{
//class adaptHuffmanBlock;
//using pAdaptHuffmanBlock = std::shared_ptr<adaptHuffmanBlock>;

template <typename Ty_>
class adaptHuffmanBlock : public flattenBlock<Ty_>
{
public:
	adaptHuffmanBlock(pBlockDesc desc)
		: flattenBlock<Ty_>(desc)
	{

	}
	virtual ~adaptHuffmanBlock()
	{

	}

public:
	template<typename Cty_, typename Ty_>
	void serializeTy(aHuffmanCoder<Cty_, Ty_>& coder, bstream& out)
	{
		auto iit = this->getItemIterator();

		while (!iit->isEnd())
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
#endif	// _MSDB_ADAPTHUFFMANBLOCK_H_