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
	evaluateFunc(nullptr)
{
}

void singlePredicate::setEvaluateFunc(eleType eType)
{
	this->evaluateFunc = this->findEvaluateFunc(eType);
	this->lTerm->setEvaluateFunc(eType);
}

bool singlePredicate::evaluate(pItemItr iit)
{
	return (this->*evaluateFunc)(iit);
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

andPredicate::andPredicate(pTerm lhs, pTerm rhs)
	: predicate(lhs, rhs), evaluateFunc(nullptr)
{
}

void andPredicate::setEvaluateFunc(eleType eType)
{
	this->evaluateFunc = this->findEvaluateFunc(eType);
	this->lTerm->setEvaluateFunc(eType);
	this->rTerm->setEvaluateFunc(eType);
}

bool andPredicate::evaluate(pItemItr iit)
{
	return (this->*evaluateFunc)(iit);
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

orPredicate::orPredicate(pTerm lhs, pTerm rhs)
	: predicate(lhs, rhs),
	evaluateFunc(nullptr)
{
}

void orPredicate::setEvaluateFunc(eleType eType)
{
	this->evaluateFunc = this->findEvaluateFunc(eType);
	this->lTerm->setEvaluateFunc(eType);
	this->rTerm->setEvaluateFunc(eType);
}

bool orPredicate::evaluate(pItemItr iit)
{
	return (this->*evaluateFunc)(iit);
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
}		// core
}		// msdb