 #pragma once
#ifndef _MSDB_OP_DECODE_RAW_ACTION_H_
#define _MSDB_OP_DECODE_RAW_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class decode_raw_action : public opAction
{
public:
	decode_raw_action();
	~decode_raw_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
	void decode_rawAttribute(pArray outArr, pAttributeDesc attrDesc, pQuery qry);
	void decode_rawChunk(pArray outArr, pChunk outChunk, attributeId attrId, pQuery qry, const size_t parentThreadId);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_DECODE_RAW_ACTION_H_