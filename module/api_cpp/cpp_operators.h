#pragma once
#ifndef _MSDB_API_CPP_OPERATORS_H_
#define _MSDB_API_CPP_OPERATORS_H_

#include <pch_op.h>
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
	class OP_API AFLOperator
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
	/* Load						*/
	/* ************************ */
	class OP_API LoadOpr : public AFLOperator
	{
	public:
		LoadOpr(Array arr);

	public:
		virtual std::shared_ptr<core::opPlan> getPlan();
		virtual std::string toString(int depth);
	};
	std::shared_ptr<LoadOpr> OP_API Load(Array arr);
}		// msdb
#endif	// _MSDB_API_CPP_OPERATORS_H_
