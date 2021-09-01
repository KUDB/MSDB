#pragma once
#ifndef _MSDB_OPPARAM_H_
#define _MSDB_OPPARAM_H_

#include <pch.h>
#include <array/arrayDesc.h>
#include <util/element.h>

#define _MSDB_MAKE_PARAM(p)		std::shared_ptr<opParam>(new p())

namespace msdb
{
namespace core
{
class opParam;
class predicate;

using pParameter = std::shared_ptr<opParam>;
using parameters = std::vector<pParameter>;

using paramSetId = size_t;

class opPlan;
using pPlan = std::shared_ptr<opPlan>;

enum class opParamType
{
	PLAN,
	ARRAY,
	ATTRIBUTE,
	DIMENSION,
	CONST_TYPE,
	INTLIST,
	PREDICATE,
	COOR,
};

class opParam : public std::enable_shared_from_this<opParam>
{
public:
	using void_pointer = std::shared_ptr<void>;

public:
	opParam();

public:
	virtual void_pointer getParam() = 0;
	virtual opParamType type() = 0;
};

class opParamArray : public opParam
{
public:
	using paramType = arrayDesc;

public:
	opParamArray(pArrayDesc desc);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pArrayDesc desc_;
};

class opParamAttr : public opParam
{
public:
	using paramType = attributeDesc;

public:
	opParamAttr(pAttributeDesc desc);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pAttributeDesc desc_;
};

class opParamDim : public opParam
{
public:
	using paramType = dimensionDesc;

public:
	opParamDim(pDimensionDesc desc);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pDimensionDesc desc_;
};

class opParamConst : public opParam
{
public:
	using paramType = stableElement;

public:
	opParamConst(pStableElement ele);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pStableElement ele_;
};

class opParamIntList : public opParam
{
public:
	using paramType = std::vector<int64_t>;

public:
	opParamIntList(std::shared_ptr<std::vector<int64_t>> eleList);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	std::shared_ptr<std::vector<int64_t>> eleList_;
};

class opParamCoor : public opParam
{
public:
	using paramType = coor;

public:
	opParamCoor(std::shared_ptr<coor> coordinate);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	std::shared_ptr<coor> coor_;
};

class opParamPredicate : public opParam
{
public:
	using paramType = predicate;

public:
	opParamPredicate(std::shared_ptr<predicate> predicate);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	std::shared_ptr<predicate> predicates_;
};

//////////////////////////////
// Placeholder classes
class opParamPlaceholder
{
public:
	opParamPlaceholder() = default;

public:
	virtual opParam::void_pointer getParam();
};

class opParamArrayPlaceholder : public opParamPlaceholder, public opParamArray
{
public:
	opParamArrayPlaceholder();

public:
	virtual opParamType type();
};

class opParamAttrPlaceholder : public opParamPlaceholder, public opParamAttr
{
public:
	opParamAttrPlaceholder();

public:
	virtual opParamType type();
};

class opParamDimPlaceholder : public opParamPlaceholder, public opParamDim
{
public:
	opParamDimPlaceholder();

public:
	virtual opParamType type();
};

class opParamConstPlaceholder : public opParamPlaceholder, public opParamConst
{
public:
	opParamConstPlaceholder();

public:
	virtual opParamType type();
};

class opParamIntListPlaceholder : public opParamPlaceholder, public opParamIntList
{
public:
	opParamIntListPlaceholder();

public:
	virtual opParamType type();
};

class opParamCoorPlaceholder : public opParamPlaceholder, public opParamCoor
{
public:
	opParamCoorPlaceholder();

public:
	virtual opParamType type();
};

//////////////////////////////
// opParamPlan
class opParamPlan : public opParam
{
public:
	using paramType = opPlan;

public:
	opParamPlan(pPlan plan);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pPlan plan_;
};

class opParamPlanPlaceholder : public opParamPlaceholder, public opParamPlan
{
public:
	opParamPlanPlaceholder();

public:
	virtual opParamType type();
};
}		// core
}		// msdb
#endif	// _MSDB_OPPARAM_H_