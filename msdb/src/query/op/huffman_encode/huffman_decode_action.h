#pragma once
#ifndef _MSDB_OP_HUFFMAN_DECODE_ACTION_H_
#define _MSDB_OP_HUFFMAN_DECODE_ACTION_H_

#include <pch.h>
#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class huffman_decode_action : public opIOAction
{
public:
	huffman_decode_action();
	virtual ~huffman_decode_action();
	virtual const char* name() override
	{
		return "huffman_decode";
	}
	virtual inline int encodingType() override
	{
		return encodingType::HUFFMAN;
	}

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_HUFFMAN_DECODE_ACTION_H_