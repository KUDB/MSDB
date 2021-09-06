#include <pch.h>
#include <api_cpp/cpp_query.h>

namespace msdb
{
/* ************************ */
/* Query					*/
/* ************************ */
Query::Query(std::shared_ptr<AFLOperator> afl)
	: status_(Status::READY), qry_(std::make_shared<core::query>(afl->getPlan()))
{

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