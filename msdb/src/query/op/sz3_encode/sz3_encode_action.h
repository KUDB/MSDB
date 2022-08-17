#pragma once
#ifndef _MSDB_OP_SZ3_ENCODE_ACTION_H_
#define _MSDB_OP_SZ3_ENCODE_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class sz3_encode_action : public opAction
{
public:
	using base_type = opAction;

public:
	sz3_encode_action();
	virtual ~sz3_encode_action();
	virtual const char* name() override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);

	void compressChunk(arrayId arrId, pAttributeDesc attrDesc, pChunk outChunk, const size_t parentThreadId);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_SZ3_ENCODE_ACTION_H_