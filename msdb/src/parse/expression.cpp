#include <pch.h>
#include <parse/expression.h>

namespace msdb
{
namespace core
{
expression::expression()
{
}
expression::~expression()
{
}
expressionAttribute::expressionAttribute(attributeId attrId)
	: attrId_(attrId)
{
}
expressionAttribute::expressionAttribute(std::string attrName)
	: attrName_(attrName), attrId_(INVALID_ATTRIBUTE_ID)
{
}
expressionAttribute::~expressionAttribute()
{
}
boost::any expressionAttribute::getValue(pItemItr iit)
{
	return (**iit);
}
boost::any expressionAttribute::getValue()
{
	_MSDB_THROW(
		_MSDB_EXCEPTIONS_MSG(
		MSDB_EC_LOGIC_ERROR, MSDB_ER_NOT_IMPLEMENTED, 
		"getValue() - Not implemented on an expressionAttributeId class"));
}
std::string expressionAttribute::toString()
{
	if (this->attrId_ != INVALID_ATTRIBUTE_ID)
	{
		return std::to_string(this->attrId_);
	}
	return this->attrName_;
}
//attributeId expressionAttributeId::getValue(pItemItr iit)
//{
//	return this->value_;
//}
expressionInteger::expressionInteger(int64_t value)
	: value_(value)
{
}

boost::any expressionInteger::getValue()
{
	return this->value_;
}
std::string expressionInteger::toString()
{
	return std::to_string(this->value_);
}
expressionConst::expressionConst()
	: expression()
{
}
boost::any expressionConst::getValue(pItemItr iit)
{
	_MSDB_THROW(
		_MSDB_EXCEPTIONS_MSG(
		MSDB_EC_LOGIC_ERROR, MSDB_ER_NOT_IMPLEMENTED,
		"getValue(pItemItr) - Not implemented on an expressionConst class"));
}
std::string expressionConst::toString()
{
	return "CONST";
}
//int64_t expressionInteger::getValue(pItemItr iit)
//{
//	return this->value_;
//}
//expressionDouble::expressionDouble(double value)
//	: value_(value)
//{
//}
//expressionDouble::~expressionDouble()
//{
//}
//double expressionDouble::getValue(pItemItr iit)
//{
//	return this->value_;
//}
}		// core
}		// msdb