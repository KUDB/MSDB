#pragma once
#ifndef _MSDB_CPP_PREDICATE_H_
#define _MSDB_CPP_PREDICATE_H_

#include <pch_op.h>
#include <parse/expression.h>
#include <parse/term.h>
#include <parse/predicate.h>


namespace msdb
{
/* ************************ */
/* Expression				*/
/* ************************ */
class OP_API ExpressionAttributeImpl
{
public:
	ExpressionAttributeImpl(core::attributeId attrId);
	ExpressionAttributeImpl(std::string attrName);

public:
	std::shared_ptr<core::expressionAttribute> getExpression();

private:
	std::shared_ptr<core::expressionAttribute> expression_;
};

std::shared_ptr<ExpressionAttributeImpl> OP_API Attribute(std::string attrName);

/* ************************ */
/* Term						*/
/* ************************ */
class OP_API TermImpl
{
public:
	TermImpl(core::pExpression exp1, core::pExpression exp2, core::termType tType);

public:
	std::shared_ptr<core::term> getTerm();

private:
	std::shared_ptr<core::term> term_;
};

std::shared_ptr<TermImpl> OP_API operator == (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b);
std::shared_ptr<TermImpl> OP_API operator == (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl>& b);
//std::shared_ptr<TermImpl> operator == (const std::string& attrName, const int64_t b);
//std::shared_ptr<TermImpl> operator == (const int64_t a, const std::string& attrName);

std::shared_ptr<TermImpl> OP_API operator < (const std::shared_ptr<ExpressionAttributeImpl>& a, const int64_t b);
std::shared_ptr<TermImpl> OP_API operator < (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b);
std::shared_ptr<TermImpl> OP_API operator <= (const std::shared_ptr<ExpressionAttributeImpl>& a, const int64_t b);
std::shared_ptr<TermImpl> OP_API operator <= (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl>& b);

//std::shared_ptr<TermImpl> operator < (const std::string& attrName, const int64_t b);
//std::shared_ptr<TermImpl> operator < (const int64_t a, const std::string& attrName);
//std::shared_ptr<TermImpl> operator <= (const std::string& attrName, const int64_t b);
//std::shared_ptr<TermImpl> operator <= (const int64_t a, const std::string& attrName);

std::shared_ptr<TermImpl> OP_API operator > (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b);
std::shared_ptr<TermImpl> OP_API operator > (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b);
std::shared_ptr<TermImpl> OP_API operator >= (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b);
std::shared_ptr<TermImpl> OP_API operator >= (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b);

//std::shared_ptr<TermImpl> operator > (const std::string& attrName, const int64_t b);
//std::shared_ptr<TermImpl> operator > (const int64_t a, const std::string& attrName);
//std::shared_ptr<TermImpl> operator >= (const std::string& attrName, const int64_t b);
//std::shared_ptr<TermImpl> operator >= (const int64_t a, const std::string& attrName);

/* ************************ */
/* Predicate				*/
/* ************************ */
class OP_API PredicateImpl
{
public:
	PredicateImpl(std::shared_ptr<core::predicate> pred);

public:
	std::shared_ptr<core::predicate> getPredicate();
	std::string toString();

private:
	std::shared_ptr<core::predicate> pred_;
};

//std::shared_ptr<PredicateImpl> operator &(std::shared_ptr<TermImpl>a, std::shared_ptr<TermImpl>b);
//std::shared_ptr<PredicateImpl> operator |(std::shared_ptr<TermImpl>a, std::shared_ptr<TermImpl>b);
}		// msdb
#endif	// _MSDB_CPP_PREDICATE_H_