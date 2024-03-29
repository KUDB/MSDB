﻿#pragma once
#ifndef _MSDB_API_CPP_OPERATORS_H_
#define _MSDB_API_CPP_OPERATORS_H_

#include <pch.h>
#include <array/arrayDesc.h>
#include <array/arrayMgr.h>
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
		
	virtual std::string toString(int depth);

protected:
	static std::string getIndentString(unsigned int depth);
	static std::string getStrStart();

private:
	core::pArrayDesc arrDesc_;
};

/* ************************ */
/* Save 					*/
/* ************************ */
class SaveOpr : public AFLOperator
{
public:
	SaveOpr(std::shared_ptr<AFLOperator> qry);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
};
std::shared_ptr<SaveOpr> Save(std::shared_ptr<AFLOperator> qry);

/* ************************ */
/* Load						*/
/* ************************ */
class LoadOpr : public AFLOperator
{
public:
	LoadOpr(Array arr);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);
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
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	Domain domain_;
};
std::shared_ptr<BetweenOpr> Between(Array arr, Domain d);
std::shared_ptr<BetweenOpr> Between(std::shared_ptr<AFLOperator> qry, Domain d);

/* ************************ */
/* Build					*/
/* ************************ */
class BuildOpr : public AFLOperator
{
public:
	BuildOpr(const core::arrayId aid, const std::string name, 
			 core::pDimensionDescs dims,
			 core::pAttributeDescs attrs);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

public:
	//BuildOpr& AddAxis(id_t dimId, std::string axis, Coordinates dim, position_t chunkSize, position_t blockSize);
	//BuildOpr& AddAttribute(id_t attrId, std::string name, core::eleType eType);
	//BuildOpr& SetArray(id_t arrId, std::string name);
};
std::shared_ptr<BuildOpr> Build(core::arrayId aid, std::string name,
								std::vector<DefDimension>, std::vector<DefAttribute>);

/* ************************ */
/* Filter					*/
/* ************************ */
class FilterOpr : public AFLOperator
{
public:
	FilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	std::shared_ptr<PredicateImpl> pred_;
};
std::shared_ptr<FilterOpr> Filter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm);

class IndexFilterOpr : public AFLOperator
{
public:
	IndexFilterOpr(std::shared_ptr<AFLOperator> qry, std::shared_ptr<PredicateImpl> pred);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	std::shared_ptr<PredicateImpl> pred_;
};
std::shared_ptr<IndexFilterOpr> IndexFilter(std::shared_ptr<AFLOperator> qry, std::shared_ptr<TermImpl> singleTerm);


/* ************************ */
/* ToBuffer					*/
/* ************************ */
class CopyToBufferOpr : public AFLOperator
{
public:
	CopyToBufferOpr(std::shared_ptr<AFLOperator> qry);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
};
std::shared_ptr<CopyToBufferOpr> CopyToBuffer(std::shared_ptr<AFLOperator> qry);

/* ************************ */
/* Consume					*/
/* ************************ */
class ConsumeOpr : public AFLOperator
{
public:
	ConsumeOpr(std::shared_ptr<AFLOperator> qry);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
};
std::shared_ptr<ConsumeOpr> Consume(std::shared_ptr<AFLOperator> qry);
}		// msdb
#endif	// _MSDB_API_CPP_OPERATORS_H_