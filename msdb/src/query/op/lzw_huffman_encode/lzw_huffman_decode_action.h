#pragma once
#ifndef _MSDB_OP_LZW_HUFFMAN_DECODE_ACTION_H_
#define _MSDB_OP_LZW_HUFFMAN_DECODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <op/lzw_huffman_encode/lzwHuffmanChunk.h>

namespace msdb
{
namespace core
{
class lzw_huffman_decode_action : public opAction
{
public:
	lzw_huffman_decode_action();
	virtual ~lzw_huffman_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry);
	void loadChunk(pQuery qry, const size_t paraentThreadId, pArray outArr, attributeId attrId, pChunk outChunk);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LZW_HUFFMAN_DECODE_ACTION_H_