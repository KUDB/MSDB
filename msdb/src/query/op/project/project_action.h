#pragma once
#ifndef _MSDB_OP_PROJECT_ACTION_H_
#define _MSDB_OP_PROJECT_ACTION_H_

#include <pch.h>
#include <query/opAction.h>

namespace msdb
{
namespace core
{
class project_action : public opAction
{
public:
	project_action();
	~project_action();

public:
	virtual const char* name() override;
	virtual pArray execute(std::vector<pArray>& inputArrays, pQuery qry) override;

//private:
//	template <class Ty_>
//	void attributeEncode(pArray inArr, pAttributeDesc attrDesc, pQuery qry)
//	{
//		// TODO::Implement attribute Encode
//	}
};
}		// core
}		// msdb
#endif	// _MSDB_OP_PROJECT_ACTION_H_