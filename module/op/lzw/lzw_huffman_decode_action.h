#pragma once
#ifndef _MSDB_OP_LZW_HUFFMAN_DECODE_ACTION_H_
#define _MSDB_OP_LZW_HUFFMAN_DECODE_ACTION_H_

#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class lzw_huffman_decode_action : public opIOAction
{
public:
	lzw_huffman_decode_action();
	virtual ~lzw_huffman_decode_action();
	virtual const char* name() override
	{
		return "lzw_huffman_decode";
	}
	virtual inline int encodingType() override
	{
		return encodingType::LZW_HUFFMAN;
	}
	virtual pArray getOutArray(pArrayDesc desc) override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LZW_HUFFMAN_DECODE_ACTION_H_