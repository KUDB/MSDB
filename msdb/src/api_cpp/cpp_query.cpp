#include <pch.h>
#include <typeinfo>
#include <api_cpp/cpp_query.h>
#include <api_cpp/cpp_operators.h>

namespace msdb
{
/* ************************ */
/* Query					*/
/* ************************ */
Query::Query(std::shared_ptr<AFLOperator> afl)
	: status_(Status::READY)
{
	/**
	 * Append CopyTo Buffer operator at the end of query automatically.
	 * 
	 * - Note: except 'consume' operator, which does not use the output result.
	 */
	if(typeid(*afl) != typeid(CopyToBufferOpr))
	{
		afl = CopyToBuffer(afl);
	}
	this->qry_ = std::make_shared<core::query>(afl->getPlan());
}

ResultArray Query::execute()
{
	if (this->qry_->submit().isOk())
	{
		return ResultArray(Context(), this->qry_);
	}

	return ResultArray(Context(), this->qry_);
}
}		// msdb