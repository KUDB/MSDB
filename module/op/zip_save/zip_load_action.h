#pragma once
#ifndef _MSDB_OP_ZIP_ACTION_H_
#define _MSDB_OP_ZIP_ACTION_H_

#include <pch_op.h>
#include <query/opIOAction.h>

namespace msdb
{
namespace core
{
class zip_load_action : public opIOAction
{
public:
	zip_load_action();
	virtual ~zip_load_action();
	virtual const char* name() override
	{
		return "zip_load";
	}
	virtual inline int encodingType() override
	{
		return encodingType::ZIP;
	}
	virtual pArray getOutArray(pArrayDesc desc) override;

public:
	pArray execute(std::vector<pArray>& inputArrays, pQuery qry);
};
}		// core
}		// msdb
#endif	// _MSDB_OP_ZIP_ACTION_H_