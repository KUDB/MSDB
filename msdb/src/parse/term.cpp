#include <pch.h>
#include <parse/term.h>

namespace msdb
{
namespace core
{
term::term(pExpression lhs, pExpression rhs, termType tType)
	: lhs_(lhs), rhs_(rhs), tType_(tType), 
	evaluateFunc(nullptr)
{
}
void term::setEvaluateFunc(eleType eType)
{
	switch(eType)
	{
	case eleType::BOOL:
		this->evaluateFunc = this->findEvaluateFunc<bool>(this->tType_);
		break;
	case eleType::CHAR:
		this->evaluateFunc = this->findEvaluateFunc<char>(this->tType_);
		break;
	case eleType::INT8:
		this->evaluateFunc = this->findEvaluateFunc<int8_t>(this->tType_);
		break;
	case eleType::INT16:
		this->evaluateFunc = this->findEvaluateFunc<int16_t>(this->tType_);
		break;
	case eleType::INT32:
		this->evaluateFunc = this->findEvaluateFunc<int32_t>(this->tType_);
		break;
	case eleType::INT64:
		this->evaluateFunc = this->findEvaluateFunc<int64_t>(this->tType_);
		break;
	case eleType::UINT8:
		this->evaluateFunc = this->findEvaluateFunc<uint8_t>(this->tType_);
		break;
	case eleType::UINT16:
		this->evaluateFunc = this->findEvaluateFunc<uint16_t>(this->tType_);
		break;
	case eleType::UINT32:
		this->evaluateFunc = this->findEvaluateFunc<uint32_t>(this->tType_);
		break;
	case eleType::UINT64:
		this->evaluateFunc = this->findEvaluateFunc<uint64_t>(this->tType_);
		break;
	//case eleType::FLOAT:
	//	this->evaluateFunc = this->findEvaluateFunc<bool>(this->tType_);
	//case eleType::DOUBLE:
	//	this->evaluateFunc = this->findEvaluateFunc<bool>(this->tType_);
	default:
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_SYSTEM_ERROR, MSDB_ER_NOT_IMPLEMENTED));
	}
	
}
}		// core
}		// msdb