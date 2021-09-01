#pragma once
#ifndef _MSDB_PREDICATE_H_
#define _MSDB_PREDICATE_H_

#include <pch.h>
#include <util/coordinate.h>
#include <parse/term.h>

namespace msdb
{
namespace core
{
class predicate;
using pPredicate = std::shared_ptr<predicate>;

class predicate : public std::enable_shared_from_this<predicate>
{
public:
	predicate(pTerm lhs, pTerm rhs);
	~predicate();

public:
	virtual void setEvaluateFunc(eleType eType) = 0;
	virtual bool evaluate(pItemItr iit) = 0;

protected:
	pTerm lTerm;
	pTerm rTerm;
};

class singlePredicate : public predicate
{
public:
	singlePredicate(pTerm mit);

public:
	virtual void setEvaluateFunc(eleType eType) override;
	virtual bool evaluate(pItemItr iit) override;

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit);
	}

private:
	typedef bool(singlePredicate::* eFunc)(pItemItr);
	eFunc findEvaluateFunc(eleType type);
	bool (singlePredicate::* evaluateFunc)(pItemItr);
};

class andPredicate : public predicate
{
public:
	andPredicate(pTerm lhs, pTerm rhs);

public:
	virtual void setEvaluateFunc(eleType eType) override;
	virtual bool evaluate(pItemItr iit) override;

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit) && this->rTerm->evaluate<Ty_>(iit);
	}

private:
	typedef bool(andPredicate::* eFunc)(pItemItr);
	eFunc findEvaluateFunc(eleType type);
	bool (andPredicate::* evaluateFunc)(pItemItr);
};

class orPredicate : public predicate
{
public:
	orPredicate(pTerm lhs, pTerm rhs);

public:
	virtual void setEvaluateFunc(eleType eType) override;

	virtual bool evaluate(pItemItr iit) override;

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit) || this->rTerm->evaluate<Ty_>(iit);
	}

private:
	typedef bool(orPredicate::* eFunc)(pItemItr);
	eFunc findEvaluateFunc(eleType type);
	bool (orPredicate::* evaluateFunc)(pItemItr);
};
}		// core
}		// msdb
#endif	// _MSDB_PREDICATE_H_