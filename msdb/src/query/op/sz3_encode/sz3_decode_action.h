#pragma once
#ifndef _MSDB_OP_SZ3_DECODE_ACTION_H_
#define _MSDB_OP_SZ3_DECODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class sz3_decode_action : public opAction
{
public:
	using base_type = opAction;

public:
	sz3_decode_action();
	virtual ~sz3_decode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
	void loadAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry);
	void loadChunk(pQuery qry, const size_t parentThreadId, pArray outArr, attributeId attrId, pChunk outChunk);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SZ3_DECODE_ACTION_H_