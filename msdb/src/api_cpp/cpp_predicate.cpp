#include <pch.h>
#include <api_cpp/cpp_predicate.h>

namespace msdb
{
/* ************************ */
/* Expression				*/
/* ************************ */
std::shared_ptr<ExpressionAttributeImpl> Attribute(std::string attrName)
{
	return std::make_shared<ExpressionAttributeImpl>(attrName);
}
ExpressionAttributeImpl::ExpressionAttributeImpl(core::attributeId attrId)
	: expression_(std::make_shared<core::expressionAttribute>(attrId))
{
}
ExpressionAttributeImpl::ExpressionAttributeImpl(std::string attrName)
	: expression_(std::make_shared<core::expressionAttribute>(attrName))

{
}
std::shared_ptr<core::expressionAttribute> ExpressionAttributeImpl::getExpression()
{
	return this->expression_;
}

/* ************************ */
/* Term						*/
/* ************************ */
TermImpl::TermImpl(core::pExpression exp1, core::pExpression exp2, core::termType tType)
	: term_(std::make_shared<core::term>(exp1, exp2, tType))
{
}

std::shared_ptr<core::term> TermImpl::getTerm()
{
	return this->term_;
}

std::shared_ptr<TermImpl> operator==(const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b)
{
	return std::make_shared<TermImpl>(a->getExpression(), std::make_shared<core::expressionInteger>(b), core::termType::EQUAL);
}
std::shared_ptr<TermImpl> operator == (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl>& b)
{
	return std::make_shared<TermImpl>(std::make_shared<core::expressionInteger>(a), b->getExpression(), core::termType::EQUAL);
}

std::shared_ptr<TermImpl> operator<(const std::shared_ptr<ExpressionAttributeImpl>& a, const int64_t b)
{
	return std::make_shared<TermImpl>(a->getExpression(), std::make_shared<core::expressionInteger>(b), core::termType::GREATER);
}
std::shared_ptr<TermImpl> operator < (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b)
{
	return std::make_shared<TermImpl>(std::make_shared<core::expressionInteger>(a), b->getExpression(), core::termType::GREATER);
}
std::shared_ptr<TermImpl> operator<=(const std::shared_ptr<ExpressionAttributeImpl>& a, const int64_t b)
{
	return std::make_shared<TermImpl>(a->getExpression(), std::make_shared<core::expressionInteger>(b), core::termType::GREATER_EQUAL);
}
std::shared_ptr<TermImpl> operator<=(const int64_t a, const std::shared_ptr<ExpressionAttributeImpl>& b)
{
	return std::make_shared<TermImpl>(std::make_shared<core::expressionInteger>(a), b->getExpression(), core::termType::GREATER_EQUAL);
}

std::shared_ptr<TermImpl> operator > (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b)
{
	return std::make_shared<TermImpl>(a->getExpression(), std::make_shared<core::expressionInteger>(b), core::termType::LESS);
}
std::shared_ptr<TermImpl> operator > (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b)
{
	return std::make_shared<TermImpl>(std::make_shared<core::expressionInteger>(a), b->getExpression(), core::termType::LESS);
}
std::shared_ptr<TermImpl> operator >= (const std::shared_ptr<ExpressionAttributeImpl> a, const int64_t b)
{
	return std::make_shared<TermImpl>(a->getExpression(), std::make_shared<core::expressionInteger>(b), core::termType::LESS_EQUAL);
}
std::shared_ptr<TermImpl> operator >= (const int64_t a, const std::shared_ptr<ExpressionAttributeImpl> b)
{
	return std::make_shared<TermImpl>(std::make_shared<core::expressionInteger>(a), b->getExpression(), core::termType::LESS_EQUAL);
}

/* ************************ */
/* Predicate				*/
/* ************************ */
PredicateImpl::PredicateImpl(std::shared_ptr<core::predicate> pred)
	: pred_(pred)
{
}

//std::shared_ptr<PredicateImpl> operator&(std::shared_ptr<TermImpl> a, std::shared_ptr<TermImpl> b)
//{
//	return std::make_shared<PredicateImpl>(std::make_shared<core::andPredicate>(a, b));
//}
//
//std::shared_ptr<PredicateImpl> operator|(std::shared_ptr<TermImpl> a, std::shared_ptr<TermImpl> b)
//{
//	return std::make_shared<PredicateImpl>(std::make_shared<core::orPredicate>(a, b));
//}
}		// msdb