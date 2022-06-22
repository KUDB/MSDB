#pragma once
#ifndef _MSDB_LZWHUFFMANBLOCK_H_
#define _MSDB_LZWHUFFMANBLOCK_H_

#include <pch.h>
#include <array/flattenChunk.h>

namespace msdb
{
namespace core
{
//class lzwHuffmanBlock;
//using pLzwHuffmanBlock = std::shared_ptr<lzwHuffmanBlock>;

template <typename Ty_>
class lzwHuffmanBlock : public flattenBlock<Ty_>
{
public:
	lzwHuffmanBlock(pBlockDesc desc)
		: flattenBlock<Ty_>(desc)
	{

	}
	virtual ~lzwHuffmanBlock()
	{

	}

};
}		// core
}		// msdb
#endif	// _MSDB_LZWHUFFMANBLOCK_H_