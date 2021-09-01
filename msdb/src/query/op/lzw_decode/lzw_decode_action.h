#pragma once
#ifndef _MSDB_OP_LZW_DECODE_ACTION_H_
#define _MSDB_OP_LZW_DECODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <compression/lzwChunk.h>

namespace msdb
{
namespace core
{
class lzw_decode_action : public opAction
{
public:
	lzw_decode_action();
	virtual ~lzw_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry);
	void loadChunk(pArray outArr, pChunk outChunk, pLzwChunk inChunk, attributeId attrId, pQuery qry, const size_t parentThreadId);
	pLzwChunk makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid);
};
}		// core
}		// msdb
#endif // _MSDB_OP_LZW_DECODE_ACTION_H_