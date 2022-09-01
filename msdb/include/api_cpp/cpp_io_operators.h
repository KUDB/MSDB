#pragma once
#ifndef _MSDB_API_CPP_IO_OPERATORS_H_
#define _MSDB_API_CPP_IO_OPERATORS_H_

#include <pch.h>
#include <api_cpp/cpp_operators.h>

namespace msdb
{
/* ************************ */
/* BuildIndexOpr			*/
/* ************************ */
class BuildIndexOpr : public AFLOperator
{
public:
	BuildIndexOpr(std::shared_ptr<AFLOperator> qry, attrIndexType indexType, core::eleDefault paramOne);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	attrIndexType indexType_;
	core::eleDefault paramOne_;
};

std::shared_ptr<BuildIndexOpr> BuildIndex(std::shared_ptr<AFLOperator> qry, attrIndexType compType, core::eleDefault paramOne);

/* ************************ */
/* SaveIndexOpr				*/
/* ************************ */
class SaveIndexOpr : public AFLOperator
{
public:
	SaveIndexOpr(Array arr, attrIndexType indexType);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	attrIndexType indexType_;
};

std::shared_ptr<SaveIndexOpr> SaveIndex(Array arr, attrIndexType compType);

/* ************************ */
/* LoadIndexOpr				*/
/* ************************ */
class LoadIndexOpr : public AFLOperator
{
public:
	LoadIndexOpr(Array arr, attrIndexType indexType);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	attrIndexType indexType_;
};

std::shared_ptr<LoadIndexOpr> LoadIndex(Array arr, attrIndexType compType);

/* ************************ */
/* CompOpr					*/
/* ************************ */
class CompOpr : public AFLOperator
{
public:
	CompOpr(std::shared_ptr<AFLOperator> qry, encodingType compType);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	encodingType compType_;
};

class CompOneParamOpr : public AFLOperator
{
public:
	CompOneParamOpr(std::shared_ptr<AFLOperator> qry, encodingType compType,
					core::eleDefault paramOne);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	encodingType compType_;
	core::eleDefault paramOne_;
};

class CompTwoParamOpr : public AFLOperator
{
public:
	CompTwoParamOpr(std::shared_ptr<AFLOperator> qry, encodingType compType,
					core::eleDefault paramOne, core::eleDefault paramTwo);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	encodingType compType_;
	core::eleDefault paramOne_;
	core::eleDefault paramTwo_;
};

std::shared_ptr<CompOpr> Comp(
	std::shared_ptr<AFLOperator> qry, encodingType compType);

std::shared_ptr<CompOneParamOpr> Comp(
	std::shared_ptr<AFLOperator> qry, encodingType compType,
	core::eleDefault paramOne);

std::shared_ptr<CompTwoParamOpr> Comp(
	std::shared_ptr<AFLOperator> qry, encodingType compType, 
	core::eleDefault paramOne, core::eleDefault paramTwo);

/* ************************ */
/* DecompOpr				*/
/* ************************ */
class DecompOpr : public AFLOperator
{
public:
	DecompOpr(Array arr, encodingType compType);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	encodingType compType_;
};

class DecompOneParamOpr : public AFLOperator
{
public:
	DecompOneParamOpr(Array arr, encodingType compType,
					  core::eleDefault paramOne);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	encodingType compType_;
	core::eleDefault paramOne_;
};

class DecompTwoParamOpr : public AFLOperator
{
public:
	DecompTwoParamOpr(Array arr, encodingType compType,
					  core::eleDefault paramOne, core::eleDefault paramTwo);

public:
	virtual std::shared_ptr<core::opPlan> getPlan();
	virtual std::string toString(int depth);

private:
	std::shared_ptr<AFLOperator> childQry_;
	encodingType compType_;
	core::eleDefault paramOne_;
	core::eleDefault paramTwo_;
};

std::shared_ptr<DecompOpr> Decomp(
	Array arr, encodingType compType);

std::shared_ptr<DecompOneParamOpr> Decomp(
	Array arr, encodingType compType,
	core::eleDefault paramOne);

std::shared_ptr<DecompTwoParamOpr> Decomp(
	Array arr, encodingType compType,
	core::eleDefault paramOne, core::eleDefault paramTwo);
}		// msdb
#endif	// _MSDB_API_CPP_IO_OPERATORS_H_