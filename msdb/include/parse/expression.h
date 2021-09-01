#pragma once
#ifndef _MSDB_EXPRESSION_H_
#define _MSDB_EXPRESSION_H_

#include <pch.h>
#include <util/coordinate.h>
#include <array/attributeId.h>

namespace msdb
{
namespace core
{
class expression;
using pExpression = std::shared_ptr<expression>;

class expression : public std::enable_shared_from_this<expression>
{
public:
	expression();
	~expression();

public:
	virtual boost::any getValue(pItemItr iit) = 0;
	virtual boost::any getValue() = 0;
};

class expressionAttributeId : public expression
{
public:
	using value_type = attributeId;

public:
	expressionAttributeId(attributeId value);
	~expressionAttributeId();

public:
	virtual boost::any getValue(pItemItr iit) override;
	virtual boost::any getValue() override;

private:
	attributeId value_;
};

class expressionConst : public expression
{
public:
	expressionConst();

public:
	virtual boost::any getValue(pItemItr iit) override;

};
class expressionInteger : public expressionConst
{
public:
	using value_type = int64_t;

public:
	expressionInteger(int64_t value);

public:
	//virtual boost::any getValue(pItemItr iit) override;
	virtual boost::any getValue() override;

private:
	int64_t value_;
};

//class expressionDouble : public expression
//{
//public:
//	using value_type = double;
//
//public:
//	expressionDouble(double value);
//	~expressionDouble();
//
//public:
//	double getValue(pItemItr iit);
//
//private:
//	double value_;
//};
}		// core
}		// msdb
#endif	// _MSDB_EXPRESSION_H_
