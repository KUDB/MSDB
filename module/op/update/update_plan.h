#pragma once
#ifndef _MSDB_OP_UPDATE_PLAN_H_
#define _MSDB_OP_UPDATE_PLAN_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <api_cpp/cpp_operators.h>
#include <api_cpp/cpp_domain.h>

namespace msdb
{
namespace core
{
class OP_API fast_update_plan : public opPlan
{
public:
	fast_update_plan();
	~fast_update_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};
namespace op
{
namespace update
{
/* ************************ */
/* Update Parameter Sets	*/
/* ************************ */
class OP_API pset_file : public opArrayParamSet
{
public:
	pset_file(parameters& pSet);
};
class OP_API pset_files : public opArrayParamSet
{
public:
	using keyType = attributeId;
	using valueType = std::string;
	using containerType = std::map<keyType, valueType>;
	using containerParamType = opParamContainer<keyType, valueType>;

public:
	pset_files(parameters& pSet);

public:
	static const int IDX_CONTAINER = 1;
	static const int IDX_START_COOR = 2;
	static const int IDX_END_COOR = 3;
};
}		// update
}		// op
}		// core

/* ************************ */
/* CPP API Operator	        */
/* ************************ */
class OP_API FastUpdateOpr : public AFLOperator
{
public:
	FastUpdateOpr(Array arr, core::parameters params);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	core::parameters params_;
};
std::shared_ptr<FastUpdateOpr> OP_API FastUpdate(Array arr, 
	const std::string filePath, 
	const Coordinates start, const Coordinates end);
std::shared_ptr<FastUpdateOpr> OP_API FastUpdate(Array arr, 
	core::op::update::pset_files::containerType attrFiles,
	const Coordinates start, const Coordinates end);
}		// msdb
#endif // _MSDB_OP_UPDATE_PLAN_H_
