#pragma once
#ifndef _MSDB_CPP_PREDICATE_H_
#define _MSDB_CPP_PREDICATE_H_

#include <pch.h>
#include <parse/expression.h>
#include <parse/term.h>
#include <parse/predicate.h>


namespace msdb
{
/* ************************ */
/* Expression				*/
/* ************************ */
class ExpressionAttributeImpl
{
public:
	ExpressionAttributeImpl(core::attributeId attrId);
	ExpressionAttributeImpl(std::string attrName);

public:
	std::shared_ptr<core::expressionAttribute> getExpression();

private:
	std::shared_ptr<core::expressionAttribute> expression_;
};

std::shared_ptr<ExpressionAttributeImpl> Attribute(std::string attrName);

/* ************************ */
/* Term						*/
/* ************************ */
class TermImpl
{
public:
	TermImpl(core::pExpression exp1, core::pExpression exp2, core::termType tType);

public:
	std::shared_ptr<core::term> getTerm();

private:
	std::shared_ptr<core::term> term_;
};

std::shared_ptr<TermImpl> operator == (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b);
std::shared_ptr<TermImpl> operator == (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl>& b);

std::shared_ptr<TermImpl> operator < (const std::shared_ptr<ExpressionAttributeImpl>& a, const int64_t b);
std::shared_ptr<TermImpl> operator < (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b);
std::shared_ptr<TermImpl> operator <= (const std::shared_ptr<ExpressionAttributeImpl>& a, const int64_t b);
std::shared_ptr<TermImpl> operator <= (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl>& b);

std::shared_ptr<TermImpl> operator > (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b);
std::shared_ptr<TermImpl> operator > (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b);
std::shared_ptr<TermImpl> operator >= (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b);
std::shared_ptr<TermImpl> operator >= (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b);

/* ************************ */
/* Predicate				*/
/* ************************ */
class PredicateImpl
{
public:
	PredicateImpl(std::shared_ptr<core::predicate> pred);

public:
	std::shared_ptr<core::predicate> getPredicate();

private:
	std::shared_ptr<core::predicate> pred_;
};

//std::shared_ptr<PredicateImpl> operator &(std::shared_ptr<TermImpl>a, std::shared_ptr<TermImpl>b);
//std::shared_ptr<PredicateImpl> operator |(std::shared_ptr<TermImpl>a, std::shared_ptr<TermImpl>b);
}		// msdb
#endif	// _MSDB_CPP_PREDICATE_H_