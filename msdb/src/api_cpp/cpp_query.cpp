#include <pch.h>
#include <api_cpp/cpp_query.h>

namespace msdb
{
/* ************************ */
/* Query					*/
/* ************************ */
Query::Query()
{
	this->status_ = Status::READY;
}

ResultArray Query::execute()
{
	// TODO::execute()
	return ResultArray(Context(), nullptr, nullptr);
}

/* ************************ */
/* AFLQuery					*/
/* ************************ */
AFLQuery::AFLQuery()
{
}
}		// msdb