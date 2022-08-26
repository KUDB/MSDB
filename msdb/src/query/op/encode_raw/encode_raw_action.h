#pragma once
#ifndef _MSDB_OP_ENCODE_RAW_ACTION_H_
#define _MSDB_OP_ENCODE_RAW_ACTION_H_

#include <pch.h>
#include <array/arrayMgr.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class encode_raw_action : public opAction
{
public:
	encode_raw_action();
	~encode_raw_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery q) override;
	void encodeChunk(arrayId arrId, attributeId attrId, pChunk outChunk, pQuery qry, const size_t parentThreadId);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ENCODE_RAW_ACTION_H_