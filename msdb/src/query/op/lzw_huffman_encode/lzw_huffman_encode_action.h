#pragma once
#ifndef _MSDB_OP_LZW_HUFFMAN_ENCODE_ACTION_H_
#define _MSDB_OP_LZW_HUFFMAN_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <op/lzw_huffman_encode/lzwHuffmanChunk.h>

namespace msdb
{
namespace core
{
class lzw_huffman_encode_action : public opAction
{
public:
	lzw_huffman_encode_action();
	virtual ~lzw_huffman_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
	void encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LZW_HUFFMAN_ENCODE_ACTION_H_