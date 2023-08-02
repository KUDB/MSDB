#include <pch.h>
#include <string>
#include <query/opParam.h>
#include <system/exceptions.h>
#include <parse/predicate.h>
#include <query/opPlan.h>

namespace msdb
{
namespace core
{

std::string to_string(const opParam& param)
{
	return param.toString();
}

opParamArray::opParamArray(pArrayDesc desc)
	: desc_(desc)
{
}
opParam::void_pointer opParamArray::getParam()
{
	return this->desc_;
}
opParamType opParamArray::type() const
{
	return opParamType::ARRAY;
}
std::string opParamArray::toString() const
{
	return this->desc_->toString();
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
opParamType opParamAttr::type() const
{
	return opParamType::ATTRIBUTE;
}
std::string opParamAttr::toString() const
{
	return this->desc_->toString();
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
opParamType opParamStringList::type() const
{
	return opParamType::STRING_LIST;
}
std::string opParamStringList::toString() const
{
	std::stringstream ss;
	for (auto& ele : *this->strList_)
		ss << ele;
	return ss.str();
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
opParamType opParamDim::type() const
{
	return opParamType::DIMENSION;
}
std::string opParamDim::toString() const
{
	return this->desc_->toString();
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
opParamType opParamConst::type() const
{
	return opParamType::CONST_TYPE;
}
std::string opParamConst::toString() const
{
	switch (this->ele_->getEleType())
	{
	case eleType::BOOL:
		break;
	case eleType::CHAR:
	case eleType::INT8:
		return std::to_string(this->ele_->getChar());
	case eleType::INT16:
		return std::to_string(this->ele_->getInt16());
	case eleType::INT32:
		return std::to_string(this->ele_->getInt32());
	case eleType::INT64:
		return std::to_string(this->ele_->getInt64());
	case eleType::UINT8:
		return std::to_string(uint8_t(this->ele_->getChar()));
	case eleType::UINT16:
		return std::to_string(uint16_t(this->ele_->getInt16()));
	case eleType::UINT32:
		return std::to_string(uint32_t(this->ele_->getInt32()));
	case eleType::UINT64:
		return std::to_string(uint64_t(this->ele_->getInt64()));
	case eleType::FLOAT:
		return std::to_string(this->ele_->getFloat());
	case eleType::DOUBLE:
		return std::to_string(this->ele_->getDouble());
	}

	return "";
}

//////////////////////////////
// opParam IntList
opParamIntList::opParamIntList(std::shared_ptr<std::vector<int64_t>> myList)
	: _myList(myList)
{
}
opParam::void_pointer opParamIntList::getParam()
{
	return this->_myList;
}
opParamType opParamIntList::type() const
{
	return opParamType::INTLIST;
}
std::string opParamIntList::toString() const
{
	std::stringstream ss;
	for (auto& ele : *this->_myList)
		ss << ele;
	return ss.str();
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
opParamType opParamPredicate::type() const
{
	return opParamType::PREDICATE;
}
std::string opParamPredicate::toString() const
{
	return this->predicates_->toString();
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
opParamType opParamConstPlaceholder::type() const
{
	return opParamType::CONST_TYPE;
}
opParamDimPlaceholder::opParamDimPlaceholder()
	: opParamPlaceholder(), opParamDim(nullptr)
{
}
opParamType opParamDimPlaceholder::type() const
{
	return opParamType::DIMENSION;
}
opParamAttrPlaceholder::opParamAttrPlaceholder()
	: opParamPlaceholder(), opParamAttr(nullptr)
{
}
opParamType opParamAttrPlaceholder::type() const
{
	return opParamType::ATTRIBUTE;
}
opParamArrayPlaceholder::opParamArrayPlaceholder()
	: opParamPlaceholder(), opParamArray(nullptr)
{
}
opParamType opParamArrayPlaceholder::type() const
{
	return opParamType::ARRAY;
}
opParamIntListPlaceholder::opParamIntListPlaceholder()
	: opParamPlaceholder(), opParamIntList(nullptr)
{
}
opParamType opParamIntListPlaceholder::type() const
{
	return opParamType::INTLIST;
}

//////////////////////////////
// opParamCoor
opParamCoor::opParamCoor(std::shared_ptr<coor> coordinate)
	: opParam(), coor_(coordinate)
{}
opParamCoor::opParamCoor(const coor& coordinates)
	: opParam(), coor_(std::make_shared<coor>(coordinates))
{}
opParam::void_pointer opParamCoor::getParam()
{
	return this->coor_;
}
opParamType opParamCoor::type() const
{
	return opParamType::COOR;
}
std::string opParamCoor::toString() const
{
	return this->coor_->toString();
}
opParamCoorPlaceholder::opParamCoorPlaceholder()
	: opParamPlaceholder(), opParamCoor(nullptr)
{
}
opParamType opParamCoorPlaceholder::type() const
{
	return opParamType::COOR;
}

//////////////////////////////
// opParamString
opParamString::opParamString(std::shared_ptr<std::string> str)
	: opParam(), str_(str)
{}
opParamString::opParamString(const std::string& str)
	: opParam(), str_(std::make_shared<std::string>(str))
{}
opParam::void_pointer opParamString::getParam()
{
	return this->str_;
}
opParamType opParamString::type() const
{
	return opParamType::STRING;
}
std::string opParamString::toString() const
{
	return *this->str_;
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
opParamType opParamPlan::type() const
{
	return opParamType::PLAN;
}
std::string opParamPlan::toString() const
{
	return this->plan_->name();
}
opParamPlanPlaceholder::opParamPlanPlaceholder()
	: opParamPlaceholder(), opParamPlan(nullptr)
{
}
opParamType opParamPlanPlaceholder::type() const
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
}
opParamType opParamMemory::type() const
{
	return opParamType::MEMORY;
}
std::string opParamMemory::toString() const
{
	std::ostringstream oss;
	// Convert pointer address to string
	oss << this->mem_;
	return oss.str();
}
}		// core
}		// msdb