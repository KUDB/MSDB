#pragma once
#ifndef _MSDB_OP_HUFFMAN_ENCODE_ACTION_H_
#define _MSDB_OP_HUFFMAN_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <compression/huffmanChunk.h>

namespace msdb
{
namespace core
{
class huffman_encode_action : public opAction
{
public:
	huffman_encode_action();
	virtual ~huffman_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	pHuffmanChunk makeOutChunk(pChunk inChunk);
};
}		// msdb
}		// core
#endif	// _MSDB_OP_HUFFMAN_ENCODE_ACTION_H_