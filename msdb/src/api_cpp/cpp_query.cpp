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
	if(typeid(*afl) != typeid(CopyToBufferOpr) && typeid(*afl) != typeid(ConsumeOpr) && typeid(*afl) != typeid(BuildOpr) && typeid(*afl) != typeid(SaveOpr))
	{
		afl = CopyToBuffer(afl);
	}
	this->qry_ = std::make_shared<core::query>(afl->getPlan());
}

ResultArray Query::execute()
{
	if (this->qry_->submit().isOk())
	{
		this->status_ = Status::COMPLETE;
		return ResultArray(Context(), this->qry_);
	}

	this->status_ = Status::FAIL;
	return ResultArray(Context(), this->qry_);
}
core::pTimer Query::getTimer()
{
	return this->qry_->getTimer();
}
std::string Query::strStatus()
{
	auto dimBuffer = this->qry_->getDimBuffer();

	std::stringstream ss;
	ss << "Query ";
	switch (this->status_)
	{
	case Status::READY:
		ss << "READY, ";
		break;
	case Status::COMPLETE:
		ss << "COMPLETE, ";
		break;
	case Status::FAIL:
		ss << "FAIL, ";
		break;
	case Status::INPROGRESS:
		ss << "INPROGRESS, ";
		break;
	default:
		break;
	};

	ss << boost::format("%1$.5f") % this->qry_->getTimer()->getExecutionTime() << " sec (";
	if (!this->qry_->isIOOperator())
	{
		ss << dimBuffer->size() << " cells, ";
	}
	ss << this->qry_->getIOBytes() << " bytes)";
	return ss.str();
}
void Query::setVerbose()
{
	this->qry_->setVerbose();
}
void Query::unsetVerbose()
{
	this->qry_->unsetVerbose();
}
}		// msdb