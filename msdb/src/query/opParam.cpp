﻿#include <pch.h>
#include <query/opParam.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
opParamArray::opParamArray(pArrayDesc desc)
	: desc_(desc)
{
}
opParam::void_pointer opParamArray::getParam()
{
	return this->desc_;
}

opParamType opParamArray::type()
{
	return opParamType::ARRAY;
}

//////////////////////////////
// opParam Attribute
opParamAttr::opParamAttr(pAttributeDesc desc)
	: desc_(desc)
{
}

opParam::void_pointer opParamAttr::getParam()
{
	return this->desc_;
}

opParamType opParamAttr::type()
{
	return opParamType::ATTRIBUTE;
}

//////////////////////////////
// opParam Attribute List
opParamStringList::opParamStringList(std::shared_ptr<std::vector<std::string>> strList)
	: strList_(strList)
{
}
opParam::void_pointer opParamStringList::getParam()
{
	return this->strList_;
}
opParamType opParamStringList::type()
{
	return opParamType::STRING_LIST;
}

//////////////////////////////
// opParam Dimension
opParamDim::opParamDim(pDimensionDesc desc)
	: desc_(desc)
{
}

opParam::void_pointer opParamDim::getParam()
{
	return this->desc_;
}

opParamType opParamDim::type()
{
	return opParamType::DIMENSION;
}

//////////////////////////////
// opParam Const
opParamConst::opParamConst(pStableElement ele)
	: opParam(), ele_(ele)
{
}

opParam::void_pointer opParamConst::getParam()
{
	return this->ele_;
}
opParamType opParamConst::type()
{
	return opParamType::CONST_TYPE;
}

//////////////////////////////
// opParam Predicate
opParamPredicate::opParamPredicate(std::shared_ptr<predicate> predicate)
	: opParam(), predicates_(predicate)
{
}
opParam::void_pointer opParamPredicate::getParam()
{
	return this->predicates_;
}
opParamType opParamPredicate::type()
{
	return opParamType::PREDICATE;
}

opParam::opParam()
{
}
opParam::void_pointer opParamPlaceholder::getParam()
{
	_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_LOGIC_ERROR, MSDB_ER_DO_NOT_USE));
	return nullptr;
}
opParamConstPlaceholder::opParamConstPlaceholder()
	: opParamPlaceholder(), opParamConst(nullptr)
{
}
opParamType opParamConstPlaceholder::type()
{
	return opParamType::CONST_TYPE;
}
opParamDimPlaceholder::opParamDimPlaceholder()
	: opParamPlaceholder(), opParamDim(nullptr)
{
}
opParamType opParamDimPlaceholder::type()
{
	return opParamType::DIMENSION;
}
opParamAttrPlaceholder::opParamAttrPlaceholder()
	: opParamPlaceholder(), opParamAttr(nullptr)
{
}
opParamType opParamAttrPlaceholder::type()
{
	return opParamType::ATTRIBUTE;
}
opParamArrayPlaceholder::opParamArrayPlaceholder()
	: opParamPlaceholder(), opParamArray(nullptr)
{
}
opParamType opParamArrayPlaceholder::type()
{
	return opParamType::ARRAY;
}
opParamIntListPlaceholder::opParamIntListPlaceholder()
	: opParamPlaceholder(), opParamIntList(nullptr)
{
}
opParamType opParamIntListPlaceholder::type()
{
	return opParamType::INTLIST;
}
opParamIntList::opParamIntList(std::shared_ptr<std::vector<int64_t>> eleList)
	: eleList_(eleList)
{
}
opParam::void_pointer opParamIntList::getParam()
{
	return this->eleList_;
}
opParamType opParamIntList::type()
{
	return opParamType::INTLIST;
}

//////////////////////////////
// opParamCoor
opParamCoor::opParamCoor(std::shared_ptr<coor> coordinate)
	: opParam(), coor_(coordinate)
{
}
opParam::void_pointer opParamCoor::getParam()
{
	return this->coor_;
}
opParamType opParamCoor::type()
{
	return opParamType::COOR;
}
opParamCoorPlaceholder::opParamCoorPlaceholder()
	: opParamPlaceholder(), opParamCoor(nullptr)
{
}
opParamType opParamCoorPlaceholder::type()
{
	return opParamType::COOR;
}

//////////////////////////////
// opParamString
opParamString::opParamString(std::shared_ptr<std::string> str)
	: opParam(), str_(str)
{
}
opParam::void_pointer opParamString::getParam()
{
	return this->str_;
}
opParamType opParamString::type()
{
	return opParamType::STRING;
}

//////////////////////////////
// opParamPlan
opParamPlan::opParamPlan(std::shared_ptr<opPlan> plan)
	: plan_(plan)
{
}
opParam::void_pointer opParamPlan::getParam()
{
	return this->plan_;
}
opParamType opParamPlan::type()
{
	return opParamType::PLAN;
}
opParamPlanPlaceholder::opParamPlanPlaceholder()
	: opParamPlaceholder(), opParamPlan(nullptr)
{
}
opParamType opParamPlanPlaceholder::type()
{
	return opParamType::PLAN;
}

//////////////////////////////
// opParamMemory
opParamMemory::opParamMemory()
	: opParam(), mem_(nullptr), size_(0)
{

}
opParamMemory::opParamMemory(std::shared_ptr<void> mem, const uint64_t size)
	: opParam(), mem_(mem), size_(size)
{
}
opParam::void_pointer opParamMemory::getParam()
{
	// TODO:: return tuple
	return std::make_shared<opParamMemory::paramType>(std::make_tuple(this->mem_, this->size_));
	//return this->mem_;

}
opParamType opParamMemory::type()
{
	return opParamType::MEMORY;
}
}		// core
}		// msdb