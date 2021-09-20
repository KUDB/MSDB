#pragma once
#ifndef _MSDB_SEHUFFMANBLOCK_H_
#define _MSDB_SEHUFFMANBLOCK_H_

#include <pch.h>
#include <array/memBlock.h>

namespace msdb
{
namespace core
{
class seHuffmanBlock;
using pSeHuffmanBlock = std::shared_ptr<seHuffmanBlock>;

class seHuffmanBlock : public memBlock
{
public:
	seHuffmanBlock(pBlockDesc desc);
	virtual ~seHuffmanBlock();

};
}		// core
}		// msdb
#endif	// _MSDB_SEHUFFMANBLOCK_H_