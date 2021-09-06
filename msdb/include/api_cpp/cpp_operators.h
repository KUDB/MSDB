#pragma once
#ifndef _MSDB_API_CPP_OPERATORS_H_
#define _MSDB_API_CPP_OPERATORS_H_

#include <pch.h>
#include <api_cpp/cpp_array.h>
#include <api_cpp/cpp_domain.h>
#include <query/opPlan.h>

namespace msdb
{
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

class LoadOpr : public AFLOperator
{
public:
	LoadOpr(Array arr);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
};
std::shared_ptr<LoadOpr> Load(Array arr);

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
}		// msdb
#endif	// _MSDB_API_CPP_OPERATORS_H_