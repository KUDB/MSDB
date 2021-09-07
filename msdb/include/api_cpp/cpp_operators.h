#pragma once
#ifndef _MSDB_API_CPP_OPERATORS_H_
#define _MSDB_API_CPP_OPERATORS_H_

#include <pch.h>
#include <api_cpp/cpp_array.h>
#include <api_cpp/cpp_domain.h>
#include <api_cpp/cpp_predicate.h>
#include <query/opPlan.h>
#include <parse/predicate.h>

namespace msdb
{
/* ************************ */
/* AFLOperator				*/
/* ************************ */
class AFLOperator
{
public:
	AFLOperator(core::pArrayDesc arrDesc);

public:
	virtual std::shared_ptr<core::opPlan> getPlan() = 0;

public:
	inline core::pArrayDesc getArrayDesc()
	{
		return this->arrDesc_;
	}

private:
	core::pArrayDesc arrDesc_;
};

/* ************************ */
/* Load						*/
/* ************************ */
class LoadOpr : public AFLOperator
{
public:
	LoadOpr(Array arr);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
};
std::shared_ptr<LoadOpr> Load(Array arr);

/* ************************ */
/* Between					*/
/* ************************ */
class BetweenOpr : public AFLOperator
{
public:
	BetweenOpr(Array arr, Domain d);
	BetweenOpr(std::shared_ptr<AFLOperator> qry, Domain d);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();

private:
	std::shared_ptr<AFLOperator> childQry_;
	Domain domain_;
};
std::shared_ptr<BetweenOpr> Between(Array arr, Domain d);
std::shared_ptr<BetweenOpr> Between(std::shared_ptr<AFLOperator> qry, Domain d);

/* ************************ */
/* Filter					*/
/* ************************ */
class FilterOpr : public AFLOperator
{
public:
	FilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();

private:
	std::shared_ptr<AFLOperator> childQry_;
	std::shared_ptr<PredicateImpl> pred_;
};
std::shared_ptr<FilterOpr> Filter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm);


/* ************************ */
/* ToBuffer					*/
/* ************************ */
class CopyToBufferOpr : public AFLOperator
{
public:
	CopyToBufferOpr(std::shared_ptr<AFLOperator> qry);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();

private:
	std::shared_ptr<AFLOperator> childQry_;
};
std::shared_ptr<CopyToBufferOpr> CopyToBuffer(std::shared_ptr<AFLOperator> qry);
}		// msdb
#endif	// _MSDB_API_CPP_OPERATORS_H_