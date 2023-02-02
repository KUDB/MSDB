#pragma once
#ifndef _MSDB_OP_HUFFMAN_ENCODE_ACTION_H_
#define _MSDB_OP_HUFFMAN_ENCODE_ACTION_H_

#include <pch_op.h>
#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class huffman_encode_action : public opIOAction
{
public:
	huffman_encode_action();
	virtual ~huffman_encode_action();
	virtual const char* name() override
	{
		return "huffman_encode";
	}
	virtual inline int encodingType() override
	{
		return encodingType::HUFFMAN;
	}
	virtual pArray getOutArray(pArrayDesc desc) override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// msdb
}		// core
#endif	// _MSDB_OP_HUFFMAN_ENCODE_ACTION_H_