#pragma once
#ifndef _MSDB_OP_INSERT_PLAN_H_
#define _MSDB_OP_INSERT_PLAN_H_

#include <pch_op.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>
#include <api_cpp/cpp_operators.h>

namespace msdb
{
namespace core
{
ENUM_CLASS_MACRO(opInsertType,
		   FILE, MEMORY);

class OP_API insert_plan : public opPlan
{
public:
	insert_plan();
	~insert_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class OP_API insert_array_file_pset : public opArrayParamSet
{
public:
	insert_array_file_pset(parameters& pSet);
};
class OP_API insert_array_multi_attr_file_pset : public opArrayParamSet
{
public:
	using keyType = attributeId;
	using valueType = opParamString;
	using containerType = std::map<keyType, valueType>;
	using containerParamType = opParamContainer<keyType, valueType>;

public:
	insert_array_multi_attr_file_pset(parameters& pSet);
};

class OP_API insert_array_multi_attr_memory_pset : public opArrayParamSet
{
public:
	using keyType = attributeId;
	using valueType = core::opParamMemory;
	using containerType = std::map<keyType, valueType>;
	using containerParamType = opParamContainer<keyType, valueType>;

public:
	insert_array_multi_attr_memory_pset(parameters& pSet);
};
}		// core

/* ************************ */
/* Insert					*/
/* ************************ */
enum OP_API InsertOprType {
	single_from_file,
	multi_from_file,
	multi_from_memory
};

class OP_API InsertOpr : public AFLOperator
{
public:
	InsertOpr(Array arr, core::parameters params, InsertOprType type);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	core::parameters params_;
	InsertOprType type_;
};
std::shared_ptr<InsertOpr> OP_API Insert(Array arr, std::string filePath);
std::shared_ptr<InsertOpr> OP_API Insert(Array arr, core::insert_array_multi_attr_file_pset::containerType attrFiles);
std::shared_ptr<InsertOpr> OP_API Insert(Array arr, core::insert_array_multi_attr_memory_pset::containerType attrMem);

}		// msdb
#endif // _MSDB_OP_INSERT_PLAN_H_
