#pragma once
#ifndef _MSDB_LZWBLOCK_H_
#define _MSDB_LZWBLOCK_H_

#include <pch.h>
#include <array/memBlock.h>
#include <util/ioutil.h>

namespace msdb
{
namespace core
{
class lzwBlock;
using pLzwBlock = std::shared_ptr<lzwBlock>;

class lzwBlock : public memBlock
{
public:
	lzwBlock(pBlockDesc desc);
	virtual ~lzwBlock();

public:
	template<typename Ty_>
	void serializeTy(std::stringstream& compressed)
	{
        
	}

	template<typename Ty_>
	void deserializeTy(std::stringstream& compressed)
	{
		
	}
};
}		// core
}		// msdb
#endif	// _MSDB_LZWBLOCK_H_