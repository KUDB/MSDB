#pragma once
#ifndef _MSDB_LZWHUFFMANBLOCK_H_
#define _MSDB_LZWHUFFMANBLOCK_H_

#include <pch.h>
#include <array/memBlock.h>

namespace msdb
{
namespace core
{
class lzwHuffmanBlock;
using pLzwHuffmanBlock = std::shared_ptr<lzwHuffmanBlock>;

class lzwHuffmanBlock : public memBlock
{
public:
	lzwHuffmanBlock(pBlockDesc desc);
	virtual ~lzwHuffmanBlock();

};
}		// core
}		// msdb
#endif	// _MSDB_LZWHUFFMANBLOCK_H_