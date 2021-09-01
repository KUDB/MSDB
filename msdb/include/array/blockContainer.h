#pragma once
#ifndef _MSDB_BLOCKCONTAINER_H_
#define _MSDB_BLOCKCONTAINER_H_

#include <pch.h>
#include <array/block.h>

namespace msdb
{
namespace core
{
//using blockContainer = std::map<blockId, pBlock>;
using blockContainer = std::vector<pBlock>;
using blockPair = std::pair<blockId, pBlock>;
}		// core
}		// msdb
#endif // _MSDB_BLOCKCONTAINER_H_
