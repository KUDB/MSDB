#pragma once
#ifndef _MSDB_OP_HUFFMAN_DECODE_ACTION_H_
#define _MSDB_OP_HUFFMAN_DECODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <compression/huffmanChunk.h>

namespace msdb
{
namespace core
{
class huffman_decode_action : public opAction
{
public:
	huffman_decode_action();
	virtual ~huffman_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry);
	void loadChunk(pArray outArr, pChunk outChunk, pHuffmanChunk inChunk, attributeId attrId, pQuery qry, const size_t paraentThreadId);
	pHuffmanChunk makeInChunk(pArray inArr, pAttributeDesc attrDesc, chunkId cid);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_HUFFMAN_DECODE_ACTION_H_