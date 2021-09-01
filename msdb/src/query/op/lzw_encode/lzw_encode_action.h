#pragma once
#ifndef _MSDB_OP_LZW_ENCODE_ACTION_H_
#define _MSDB_OP_LZW_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>
#include <util/math.h>
#include <compression/lzwChunk.h>

namespace msdb
{
namespace core
{
class lzw_encode_action : public opAction
{
public:
	lzw_encode_action();
	virtual ~lzw_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

private:
	pLzwChunk makeOutChunk(pChunk inChunk);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_LZW_ENCODE_ACTION_H_