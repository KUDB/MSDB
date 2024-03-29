﻿#pragma once
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

enum class opParamType
{
	PLAN,			// msdb::core::opPlan (query plan)
	CONTAINER,		// std::map<TyKey_, TyValue_>
	ARRAY,			// msdb::core::arrayDesc (array)
	ATTRIBUTE,		// msdb::core::attributeDesc (attribute)
	DIMENSION,		// msdb::core::dimensionDesc (dimensions)
	CONST_TYPE,		// msdb::core::pStableElement, - deprecated - 
	INTLIST,		// std::vector<int64_t>
	STRING_LIST,	// std::vector<std::string>
	PREDICATE,		// msdb::core::predicate
	COOR,			// msdb::core::coor
	STRING,			// std::string
	ENUM,			// - deprecated - 
	MEMORY			// std::tuple<std::shared_ptr<void>, uint64_t>
					// -> mem_, size_
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

class opParamStringList : public opParam
{
public:
	using paramType = std::vector<std::string>;

public:
	opParamStringList(std::shared_ptr<std::vector<std::string>> strList);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	std::shared_ptr<std::vector<std::string>> strList_;
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

class opParamString : public opParam
{
public:
	using paramType = std::string;

public:
	opParamString(std::shared_ptr<std::string> str);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	std::shared_ptr<std::string> str_;
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

// Make 
// std::shared_ptr<Ty> ptr(new Ty[10], std::default_delete<Ty[]>());
class opParamMemory : public opParam
{
public:
	using paramType = std::tuple<std::shared_ptr<void>, uint64_t>;

public:
	/////
	// Default constructor
	//
	// default constructor is required to use the class in 'map' container with 'operator[]'
	//
	// data_type& operator[](const key_type& k)
	// operator[] inserts the default object data_type()
	opParamMemory();

	/**
	 * memSize: size of memory (== length * sizeof(Ty_))
	 */
	opParamMemory(std::shared_ptr<void> mem, const uint64_t memSize);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();
	//inline uint64_t getMemsize()
	//{
	//	return this->size_;
	//}

private:
	std::shared_ptr<void> mem_;
	uint64_t size_;
};

template <typename Ty_>
std::shared_ptr<Ty_> makeParamMemory(size_t bytes)
{
	return std::shared_ptr<Ty_>(new Ty_[bytes / sizeof(Ty_)], std::default_delete<Ty_[]>());
}

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
	opParamPlan(std::shared_ptr<opPlan> plan);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	std::shared_ptr<opPlan> plan_;
};

class opParamPlanPlaceholder : public opParamPlaceholder, public opParamPlan
{
public:
	opParamPlanPlaceholder();

public:
	virtual opParamType type();
};

template <typename TyEnum_>
class opParamEnum : public opParam
{
public:
	using paramType = int64_t;

public:
	opParamEnum(TyEnum_ value);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	std::shared_ptr<TyEnum_> value_;
};

template <typename TyKey_, typename TyValue_>
class opParamContainer : public opParam
{
public:
	using paramType = std::map<TyKey_, TyValue_>;
	using pContainer = std::shared_ptr<paramType>;

public:
	opParamContainer(pContainer container);

public:
	virtual opParam::void_pointer getParam();
	virtual opParamType type();

private:
	pContainer container_;
};
}		// core
}		// msdb

#include "opParam.hpp"

#endif	// _MSDB_OPPARAM_H_