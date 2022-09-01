#pragma once
#ifndef _MSDB_OP_INSERT_PLAN_H_
#define _MSDB_OP_INSERT_PLAN_H_

#include <pch.h>
#include <query/opPlan.h>
#include <query/opParamSet.h>

namespace msdb
{
namespace core
{
ENUM_CLASS_MACRO(opInsertType,
		   FILE, MEMORY);

class insert_plan : public opPlan
{
public:
	insert_plan();
	~insert_plan();

public:
	virtual const char* name() override;
	virtual pAction makeAction() override;
};

class insert_array_file_pset : public opArrayParamSet
{
public:
	insert_array_file_pset(parameters& pSet);
};
class insert_array_multi_attr_file_pset : public opArrayParamSet
{
public:
	using keyType = attributeId;
	using valueType = opParamString;
	using containerType = std::map<keyType, valueType>;
	using containerParamType = opParamContainer<keyType, valueType>;

public:
	insert_array_multi_attr_file_pset(parameters& pSet);
};

class insert_array_multi_attr_memory_pset : public opArrayParamSet
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
}		// msdb
#endif // _MSDB_OP_INSERT_PLAN_H_
