#pragma once
#ifndef _MSDB_PREDICATE_H_
#define _MSDB_PREDICATE_H_

#include <pch.h>
#include <util/coordinate.h>
#include <parse/term.h>
#include <index/mmtNode.h>
#include <index/compass.h>

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
	virtual bool evaluateNode(pMmtNode node) = 0;
	virtual bool evaluateCompassBin(pCompassBlockIndex) = 0;
	virtual std::string toString() = 0;

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
	virtual bool evaluateNode(pMmtNode node) override;
	virtual bool evaluateCompassBin(pCompassBlockIndex bin) override;
	virtual std::string toString();

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit);
	}

	template <typename Ty_>
	bool evaluateNodeImpl(pMmtNode node)
	{
		return this->lTerm->evaluateNode<Ty_>(node);
	}

	template <typename Ty_>
	bool evaluateBinImpl(pCompassBlockIndex bin)
	{
		return this->lTerm->evaluateCompassBin<Ty_>(bin);
	}

private:
	typedef bool(singlePredicate::* eFunc)(pItemItr);
	typedef bool(singlePredicate::* enFunc)(pMmtNode);
	typedef bool(singlePredicate::* ebFunc)(pCompassBlockIndex);
	eFunc findEvaluateFunc(eleType type);
	enFunc findEvaluateNodeFunc(eleType type);
	ebFunc findEvaluateBinFunc(eleType type);
	bool (singlePredicate::* evaluateFunc)(pItemItr);
	bool (singlePredicate::* evaluateNodeFunc)(pMmtNode);
	bool (singlePredicate::* evaluateBinFunc)(pCompassBlockIndex);
};

class andPredicate : public predicate
{
public:
	andPredicate(pTerm lhs, pTerm rhs);

public:
	virtual void setEvaluateFunc(eleType eType) override;
	virtual bool evaluate(pItemItr iit) override;
	virtual bool evaluateNode(pMmtNode node) override;
	virtual bool evaluateCompassBin(pCompassBlockIndex bin) override;
	virtual std::string toString();

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit) && this->rTerm->evaluate<Ty_>(iit);
	}

	template <typename Ty_>
	bool evaluateNodeImpl(pMmtNode node)
	{
		return this->lTerm->evaluateNode<Ty_>(node);
	}

	template <typename Ty_>
	bool evaluateBinImpl(pCompassBlockIndex bin)
	{
		return this->lTerm->evaluateCompassBin<Ty_>(bin);
	}

private:
	typedef bool(andPredicate::* eFunc)(pItemItr);
	typedef bool(andPredicate::* enFunc)(pMmtNode);
	typedef bool(andPredicate::* ebFunc)(pCompassBlockIndex);
	eFunc findEvaluateFunc(eleType type);
	enFunc findEvaluateNodeFunc(eleType type);
	ebFunc findEvaluateBinFunc(eleType type);
	bool (andPredicate::* evaluateFunc)(pItemItr);
	bool (andPredicate::* evaluateNodeFunc)(pMmtNode);
	bool (andPredicate::* evaluateBinFunc)(pCompassBlockIndex);
};

class orPredicate : public predicate
{
public:
	orPredicate(pTerm lhs, pTerm rhs);

public:
	virtual void setEvaluateFunc(eleType eType) override;
	virtual bool evaluate(pItemItr iit) override;
	virtual bool evaluateNode(pMmtNode node) override;
	virtual bool evaluateCompassBin(pCompassBlockIndex bin) override;
	virtual std::string toString();

	template <typename Ty_>
	bool evaluateImpl(pItemItr iit)
	{
		return this->lTerm->evaluate<Ty_>(iit) || this->rTerm->evaluate<Ty_>(iit);
	}

	template <typename Ty_>
	bool evaluateNodeImpl(pMmtNode node)
	{
		return this->lTerm->evaluateNode<Ty_>(node);
	}

	template <typename Ty_>
	bool evaluateBinImpl(pCompassBlockIndex bin)
	{
		return this->lTerm->evaluateCompassBin<Ty_>(bin);
	}

private:
	typedef bool(orPredicate::* eFunc)(pItemItr);
	typedef bool(orPredicate::* enFunc)(pMmtNode);
	typedef bool(orPredicate::* ebFunc)(pCompassBlockIndex);
	eFunc findEvaluateFunc(eleType type);
	enFunc findEvaluateNodeFunc(eleType type);
	ebFunc findEvaluateBinFunc(eleType type);
	bool (orPredicate::* evaluateFunc)(pItemItr);
	bool (orPredicate::* evaluateNodeFunc)(pMmtNode);
	bool (orPredicate::* evaluateBinFunc)(pCompassBlockIndex);
};
}		// core
}		// msdb
#endif	// _MSDB_PREDICATE_H_