#pragma once
#ifndef _MSDB_OPPARAMSET_H_
#define _MSDB_OPPARAMSET_H_

#include <pch.h>
#include <query/opParam.h>
#include <index/bitmap.h>

namespace msdb
{
namespace core
{
class opParamSet;
using pParamSet = std::shared_ptr<opParamSet>;

class opParamSet : public std::enable_shared_from_this <opParamSet>
{
public:
	opParamSet();
	opParamSet(parameters& pSet);

public:
	//bool isMatch(parameters pSet);
	virtual pArrayDesc inferSchema() = 0;
	virtual pBitmapTree inferBottomUpBitmap() = 0;
	virtual pBitmapTree inferTopDownBitmap(pBitmapTree fromParent) = 0;
	parameters getParam();

protected:
	parameters params_;
};

class opArrayParamSet : public opParamSet
{
public:
	opArrayParamSet(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;
	virtual pBitmapTree inferTopDownBitmap(pBitmapTree fromParent) override;
};

class opPlanParamSet : public opParamSet
{
public:
	opPlanParamSet(parameters& pSet);

public:
	virtual pArrayDesc inferSchema() override;
	virtual pBitmapTree inferBottomUpBitmap() override;
	virtual pBitmapTree inferTopDownBitmap(pBitmapTree fromParent) override;

public:
	pBitmapTree getSourcePlanBottomUpBitmap();
};
}		// core
}		// msdb
#endif	// _MSDB_OPPARAMSET_H_
