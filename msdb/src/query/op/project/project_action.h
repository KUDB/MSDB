#pragma once
#ifndef _MSDB_OP_PROJECT_ACTION_H_
#define _MSDB_OP_PROJECT_ACTION_H_

#include <query/opAction.h>

namespace msdb
{
namespace op
{
using namespace msdb::core;

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
}		// op
}		// msdb
#endif	// _MSDB_OP_PROJECT_ACTION_H_