#include <pch.h>
#include <parse/predicate.h>

namespace msdb
{
namespace core
{
predicate::predicate(pTerm lhs, pTerm rhs)
	: lTerm(lhs), rTerm(rhs)
{
}
predicate::~predicate()
{
}


singlePredicate::singlePredicate(pTerm mit)
	: predicate(mit, nullptr),
	evaluateFunc(nullptr), evaluateNodeFunc(nullptr), evaluateBinFunc(nullptr)
{
}

void singlePredicate::setEvaluateFunc(eleType eType)
{
	this->evaluateFunc = this->findEvaluateFunc(eType);
	this->evaluateNodeFunc = this->findEvaluateNodeFunc(eType);
	this->evaluateBinFunc = this->findEvaluateBinFunc(eType);
	this->lTerm->setEvaluateFunc(eType);
}

bool singlePredicate::evaluate(pItemItr iit)
{
	return (this->*evaluateFunc)(iit);
}

bool singlePredicate::evaluateNode(pMmtNode node)
{
	return (this->*evaluateNodeFunc)(node);
}

bool singlePredicate::evaluateCompassBin(pCompassBlockIndex bin)
{
	return (this->*evaluateBinFunc)(bin);
}

std::string singlePredicate::toString()
{
	return this->lTerm->toString();
}

singlePredicate::eFunc singlePredicate::findEvaluateFunc(eleType type)
{
	static bool (singlePredicate:: * func_ptr[12])(pItemItr) =
	{
		nullptr,
		&singlePredicate::evaluateImpl<bool>,
		&singlePredicate::evaluateImpl<char>,
		&singlePredicate::evaluateImpl<int8_t>,
		&singlePredicate::evaluateImpl<int16_t>,
		&singlePredicate::evaluateImpl<int32_t>,
		&singlePredicate::evaluateImpl<int64_t>,
		&singlePredicate::evaluateImpl<uint8_t>,
		&singlePredicate::evaluateImpl<uint16_t>,
		&singlePredicate::evaluateImpl<uint32_t>,
		&singlePredicate::evaluateImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

singlePredicate::enFunc singlePredicate::findEvaluateNodeFunc(eleType type)
{
	static bool (singlePredicate:: * func_ptr[12])(pMmtNode) =
	{
		nullptr,
		&singlePredicate::evaluateNodeImpl<bool>,
		&singlePredicate::evaluateNodeImpl<char>,
		&singlePredicate::evaluateNodeImpl<int8_t>,
		&singlePredicate::evaluateNodeImpl<int16_t>,
		&singlePredicate::evaluateNodeImpl<int32_t>,
		&singlePredicate::evaluateNodeImpl<int64_t>,
		&singlePredicate::evaluateNodeImpl<uint8_t>,
		&singlePredicate::evaluateNodeImpl<uint16_t>,
		&singlePredicate::evaluateNodeImpl<uint32_t>,
		&singlePredicate::evaluateNodeImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

singlePredicate::ebFunc singlePredicate::findEvaluateBinFunc(eleType type)
{
	static bool (singlePredicate:: * func_ptr[12])(pCompassBlockIndex) =
	{
		nullptr,
		&singlePredicate::evaluateBinImpl<bool>,
		&singlePredicate::evaluateBinImpl<char>,
		&singlePredicate::evaluateBinImpl<int8_t>,
		&singlePredicate::evaluateBinImpl<int16_t>,
		&singlePredicate::evaluateBinImpl<int32_t>,
		&singlePredicate::evaluateBinImpl<int64_t>,
		&singlePredicate::evaluateBinImpl<uint8_t>,
		&singlePredicate::evaluateBinImpl<uint16_t>,
		&singlePredicate::evaluateBinImpl<uint32_t>,
		&singlePredicate::evaluateBinImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

andPredicate::andPredicate(pTerm lhs, pTerm rhs)
	: predicate(lhs, rhs),
	evaluateFunc(nullptr), evaluateNodeFunc(nullptr), evaluateBinFunc(nullptr)
{
}

void andPredicate::setEvaluateFunc(eleType eType)
{
	this->evaluateFunc = this->findEvaluateFunc(eType);
	this->evaluateNodeFunc = this->findEvaluateNodeFunc(eType);
	this->evaluateBinFunc = this->findEvaluateBinFunc(eType);
	this->lTerm->setEvaluateFunc(eType);
	this->rTerm->setEvaluateFunc(eType);
}

bool andPredicate::evaluate(pItemItr iit)
{
	return (this->*evaluateFunc)(iit);
}

bool andPredicate::evaluateNode(pMmtNode node)
{
	return (this->*evaluateNodeFunc)(node);
}

bool andPredicate::evaluateCompassBin(pCompassBlockIndex bin)
{
	return (this->*evaluateBinFunc)(bin);
}

std::string andPredicate::toString()
{
	std::stringstream ss;
	ss << this->lTerm->toString() << " AND " << this->rTerm->toString();
	return ss.str();
}

andPredicate::eFunc andPredicate::findEvaluateFunc(eleType type)
{
	static bool (andPredicate:: * func_ptr[12])(pItemItr) =
	{
		nullptr,
		&andPredicate::evaluateImpl<bool>,
		&andPredicate::evaluateImpl<char>,
		&andPredicate::evaluateImpl<int8_t>,
		&andPredicate::evaluateImpl<int16_t>,
		&andPredicate::evaluateImpl<int32_t>,
		&andPredicate::evaluateImpl<int64_t>,
		&andPredicate::evaluateImpl<uint8_t>,
		&andPredicate::evaluateImpl<uint16_t>,
		&andPredicate::evaluateImpl<uint32_t>,
		&andPredicate::evaluateImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

andPredicate::enFunc andPredicate::findEvaluateNodeFunc(eleType type)
{
	static bool (andPredicate:: * func_ptr[12])(pMmtNode) =
	{
		nullptr,
		&andPredicate::evaluateNodeImpl<bool>,
		&andPredicate::evaluateNodeImpl<char>,
		&andPredicate::evaluateNodeImpl<int8_t>,
		&andPredicate::evaluateNodeImpl<int16_t>,
		&andPredicate::evaluateNodeImpl<int32_t>,
		&andPredicate::evaluateNodeImpl<int64_t>,
		&andPredicate::evaluateNodeImpl<uint8_t>,
		&andPredicate::evaluateNodeImpl<uint16_t>,
		&andPredicate::evaluateNodeImpl<uint32_t>,
		&andPredicate::evaluateNodeImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

andPredicate::ebFunc andPredicate::findEvaluateBinFunc(eleType type)
{
	static bool (andPredicate:: * func_ptr[12])(pCompassBlockIndex) =
	{
		nullptr,
		&andPredicate::evaluateBinImpl<bool>,
		&andPredicate::evaluateBinImpl<char>,
		&andPredicate::evaluateBinImpl<int8_t>,
		&andPredicate::evaluateBinImpl<int16_t>,
		&andPredicate::evaluateBinImpl<int32_t>,
		&andPredicate::evaluateBinImpl<int64_t>,
		&andPredicate::evaluateBinImpl<uint8_t>,
		&andPredicate::evaluateBinImpl<uint16_t>,
		&andPredicate::evaluateBinImpl<uint32_t>,
		&andPredicate::evaluateBinImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

orPredicate::orPredicate(pTerm lhs, pTerm rhs)
	: predicate(lhs, rhs),
	evaluateFunc(nullptr), evaluateNodeFunc(nullptr), evaluateBinFunc(nullptr)
{
}

void orPredicate::setEvaluateFunc(eleType eType)
{
	this->evaluateFunc = this->findEvaluateFunc(eType);
	this->evaluateNodeFunc = this->findEvaluateNodeFunc(eType);
	this->evaluateBinFunc = this->findEvaluateBinFunc(eType);
	this->lTerm->setEvaluateFunc(eType);
	this->rTerm->setEvaluateFunc(eType);
}

bool orPredicate::evaluate(pItemItr iit)
{
	return (this->*evaluateFunc)(iit);
}

bool orPredicate::evaluateNode(pMmtNode node)
{
	return (this->*evaluateNodeFunc)(node);
}

bool orPredicate::evaluateCompassBin(pCompassBlockIndex bin)
{
	return (this->*evaluateBinFunc)(bin);
}

std::string orPredicate::toString()
{
	std::stringstream ss;
	ss << this->lTerm->toString() << " OR " << this->rTerm->toString();
	return ss.str();
}

orPredicate::eFunc orPredicate::findEvaluateFunc(eleType type)
{
	static bool (orPredicate:: * func_ptr[12])(pItemItr) =
	{
		nullptr,
		&orPredicate::evaluateImpl<bool>,
		&orPredicate::evaluateImpl<char>,
		&orPredicate::evaluateImpl<int8_t>,
		&orPredicate::evaluateImpl<int16_t>,
		&orPredicate::evaluateImpl<int32_t>,
		&orPredicate::evaluateImpl<int64_t>,
		&orPredicate::evaluateImpl<uint8_t>,
		&orPredicate::evaluateImpl<uint16_t>,
		&orPredicate::evaluateImpl<uint32_t>,
		&orPredicate::evaluateImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

orPredicate::enFunc orPredicate::findEvaluateNodeFunc(eleType type)
{
	static bool (orPredicate:: * func_ptr[12])(pMmtNode) =
	{
		nullptr,
		&orPredicate::evaluateNodeImpl<bool>,
		&orPredicate::evaluateNodeImpl<char>,
		&orPredicate::evaluateNodeImpl<int8_t>,
		&orPredicate::evaluateNodeImpl<int16_t>,
		&orPredicate::evaluateNodeImpl<int32_t>,
		&orPredicate::evaluateNodeImpl<int64_t>,
		&orPredicate::evaluateNodeImpl<uint8_t>,
		&orPredicate::evaluateNodeImpl<uint16_t>,
		&orPredicate::evaluateNodeImpl<uint32_t>,
		&orPredicate::evaluateNodeImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}

orPredicate::ebFunc orPredicate::findEvaluateBinFunc(eleType type)
{
	static bool (orPredicate:: * func_ptr[12])(pCompassBlockIndex) =
	{
		nullptr,
		&orPredicate::evaluateBinImpl<bool>,
		&orPredicate::evaluateBinImpl<char>,
		&orPredicate::evaluateBinImpl<int8_t>,
		&orPredicate::evaluateBinImpl<int16_t>,
		&orPredicate::evaluateBinImpl<int32_t>,
		&orPredicate::evaluateBinImpl<int64_t>,
		&orPredicate::evaluateBinImpl<uint8_t>,
		&orPredicate::evaluateBinImpl<uint16_t>,
		&orPredicate::evaluateBinImpl<uint32_t>,
		&orPredicate::evaluateBinImpl<uint64_t>
	};

	return func_ptr[static_cast<int>(type)];
}
}		// core
}		// msdb