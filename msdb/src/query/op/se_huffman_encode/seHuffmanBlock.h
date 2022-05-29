#pragma once
#ifndef _MSDB_SEHUFFMANBLOCK_H_
#define _MSDB_SEHUFFMANBLOCK_H_

#include <pch.h>
#include <array/flattenChunk.h>

namespace msdb
{
namespace core
{
//class seHuffmanBlock;
//using pSeHuffmanBlock = std::shared_ptr<seHuffmanBlock>;

template <typename Ty_>
class seHuffmanBlock : public flattenBlock<Ty_>
{
public:
	seHuffmanBlock(pBlockDesc desc)
		: flattenBlock<Ty_>(desc)
	{

	}

	virtual ~seHuffmanBlock()
	{

	}

};
}		// core
}		// msdb
#endif	// _MSDB_SEHUFFMANBLOCK_H_