#pragma once
#ifndef _MSDB_TERM_H_
#define _MSDB_TERM_H_

#include <pch.h>
#include <util/coordinate.h>
#include <parse/expression.h>
#include <index/mmtNode.h>
#include <index/compass.h>

namespace msdb
{
namespace core
{
class term;
using pTerm = std::shared_ptr<term>;

enum class termType
{
	EQUAL,
	NOT_EQUAL,
	GREATER,
	GREATER_EQUAL,
	LESS,
	LESS_EQUAL
};


class term : std::enable_shared_from_this<term>
{
public:
	term(pExpression lhs, pExpression rhs, termType tType);

public:
	void setEvaluateFunc(eleType eType);
	virtual std::string toString();

	template <typename Ty_>
	bool evaluate(pItemItr iit)
	{
		// TODO:: change fixed values position
		// Current: attributeVaue(left), int64(right)
		return (this->*evaluateFunc)((**iit).get<Ty_>(), boost::any_cast<int64_t>(rhs_->getValue()));
	}

	template <typename Ty_>
	bool evaluateNode(pMmtNode node)
	{
		return (this->*evaluateNodeFunc)(node, boost::any_cast<int64_t>(rhs_->getValue()));
	}

	template <typename Ty_>
	bool evaluateCompassBin(pCompassBlockIndex bin)
	{
		return (this->*evaluateBinFunc)(bin, boost::any_cast<int64_t>(rhs_->getValue()));
	}

	template <typename Ty_>
	bool evaluateEqual(const int64_t v1, const int64_t v2)
	{
		return v1 == v2;
	}

	template <typename Ty_>
	bool evaluateNotEqual(const int64_t v1, const int64_t v2)
	{
		return v1 != v2;
	}

	template <typename Ty_>
	bool evaluateGreater(const int64_t v1, const int64_t v2)
	{
		return v1 < v2;
	}

	template <typename Ty_>
	bool evaluateGreaterEqual(const int64_t v1, const int64_t v2)
	{
		return v1 <= v2;
	}

	template <typename Ty_>
	bool evaluateLess(const int64_t v1, const int64_t v2)
	{
		return v1 > v2;
	}

	template <typename Ty_>
	bool evaluateLessEqual(const int64_t v1, const int64_t v2)
	{
		return v1 >= v2;
	}

	////////////////////
	// Node
	template <typename Ty_>
	bool evaluateNodeEqual(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMin<Ty_>() <= v2 && v2 <= v1->getMax<Ty_>();
	}

	template <typename Ty_>
	bool evaluateNodeNotEqual(const pMmtNode v1, const int64_t v2)
	{
		return v2 < v1->getMin<Ty_>() || v1->getMax<Ty_>() < v2;
	}

	template <typename Ty_>
	bool evaluateNodeGreater(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMin<Ty_>() < v2;
	}

	template <typename Ty_>
	bool evaluateNodeGreaterEqual(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMin<Ty_>() <= v2;
	}

	template <typename Ty_>
	bool evaluateNodeLess(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMax<Ty_>() > v2;
	}

	template <typename Ty_>
	bool evaluateNodeLessEqual(const pMmtNode v1, const int64_t v2)
	{
		return v1->getMax<Ty_>() >= v2;
	}

	////////////////////
	// Compass Bin
	template <typename Ty_>
	bool evaluateBinEqual(const pCompassBlockIndex v1, const int64_t v2)
	{
		return v1->at(v1->getBinIndexForValue(v2)).numElements > 0;
	}

	template <typename Ty_>
	bool evaluateBinNotEqual(const pCompassBlockIndex v1, const int64_t v2)
	{
		for (size_t i = 0; i < v1->getNumBins(); ++i)
		{
			if (v1->at(i).numElements > 0)
			{
				return true;
			}
		}
		return false;
	}

	template <typename Ty_>
	bool evaluateBinGreater(const pCompassBlockIndex v1, const int64_t v2)
	{
		for (size_t i = v1->getBinIndexForValue(v2); i < v1->getNumBins(); ++i)
		{
			if (v1->at(i).numElements > 0)
			{
				return true;
			}
		}
		return false;
	}

	template <typename Ty_>
	bool evaluateBinGreaterEqual(const pCompassBlockIndex v1, const int64_t v2)
	{
		for (size_t i = v1->getBinIndexForValue(v2); i < v1->getNumBins(); ++i)
		{
			if (v1->at(i).numElements > 0)
			{
				return true;
			}
		}
		return false;
	}

	template <typename Ty_>
	bool evaluateBinLess(const pCompassBlockIndex v1, const int64_t v2)
	{
		auto lastBin = v1->getBinIndexForValue(v2);
		for (size_t i = 0; i < lastBin; ++i)
		{
			if (v1->at(i).numElements > 0)
			{
				return true;
			}
		}
		return false;
	}

	template <typename Ty_>
	bool evaluateBinLessEqual(const pCompassBlockIndex v1, const int64_t v2)
	{
		auto lastBin = v1->getBinIndexForValue(v2);
		for (size_t i = 0; i < lastBin; ++i)
		{
			if (v1->at(i).numElements > 0)
			{
				return true;
			}
		}
		return false;
	}

protected:
	pExpression lhs_;
	pExpression rhs_;
	bool result_;

private:
	termType tType_;

	typedef bool(term::* eFunc)(const int64_t, const int64_t);
	bool (term::* evaluateFunc)(const int64_t, const int64_t);
	template <typename Ty_>
	eFunc findEvaluateFunc(termType type)
	{
		static bool (term:: * func_ptr[6])(const int64_t, const int64_t) = {
			& term::evaluateEqual<Ty_>,
			& term::evaluateNotEqual<Ty_>,
			& term::evaluateGreater<Ty_>,
			& term::evaluateGreaterEqual<Ty_>,
			& term::evaluateLess<Ty_>,
			& term::evaluateLessEqual<Ty_>
		};

		return func_ptr[static_cast<int>(type)];
	}

	typedef bool(term::* enFunc)(const pMmtNode, const int64_t);
	bool (term::* evaluateNodeFunc)(const pMmtNode, const int64_t);
	template <typename Ty_>
	enFunc findEvaluateNodeFunc(termType type)
	{
		static bool (term:: * func_ptr[6])(const pMmtNode, const int64_t) = {
			&term::evaluateNodeEqual<Ty_>,
			&term::evaluateNodeNotEqual<Ty_>,
			&term::evaluateNodeGreater<Ty_>,
			&term::evaluateNodeGreaterEqual<Ty_>,
			&term::evaluateNodeLess<Ty_>,
			&term::evaluateNodeLessEqual<Ty_>
		};

		return func_ptr[static_cast<int>(type)];
	}

	typedef bool(term::* ebFunc)(const pCompassBlockIndex, const int64_t);
	bool (term::* evaluateBinFunc)(const pCompassBlockIndex, const int64_t);
	template <typename Ty_>
	ebFunc findEvaluateBinFunc(termType type)
	{
		static bool (term:: * func_ptr[6])(const pCompassBlockIndex, const int64_t) = {
			&term::evaluateBinEqual<Ty_>,
			&term::evaluateBinNotEqual<Ty_>,
			&term::evaluateBinGreater<Ty_>,
			&term::evaluateBinGreaterEqual<Ty_>,
			&term::evaluateBinLess<Ty_>,
			&term::evaluateBinLessEqual<Ty_>
		};

		return func_ptr[static_cast<int>(type)];
	}
};
}		// core
}		// msdb
#endif	// _MSDB_TERM_H_
