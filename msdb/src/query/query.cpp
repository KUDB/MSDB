#include <pch.h>
#include <query/query.h>
#include <query/queryMgr.h>
#include <query/opPlan.h>

namespace msdb
{
namespace core
{
query::query(std::shared_ptr<opPlan> qryPlan)
	: qryPlan_(qryPlan), timer_(std::make_shared<timer>()), 
	arrDesc_(nullptr), outArr_(nullptr), 
	dimBuffer_(std::make_shared<std::vector<coor>>()), verbose_(false),
	ioOperator_(false), ioBytes_(0), rawResultOut_(false)
{

}

status query::submit()
{
	return queryMgr::instance()->querySubmit(shared_from_this());
}
status query::process()
{
	_MSDB_TRY_BEGIN
	{
		this->arrDesc_ = this->qryPlan_->inferSchema();
		this->outArr_ = this->qryPlan_->process(shared_from_this());
	}
	_MSDB_CATCH(const msdb_exception& e)
	{
		std::stringstream ss;
		ss << "* Error (MSDB Exception) in query processing *\n" << e.what() << std::endl;;
		ss << e._error_category_msg << "(" << std::to_string(e._error_category) << ")" << std::endl;;
		ss << e._error_msg << "(" << std::to_string(e._error_code) << ")" << std::endl;;
		ss << "File: " << e._file << " /Line: " << std::to_string(e._line) << "/Func: " << e._function << std::endl;;

		BOOST_LOG_TRIVIAL(error) << ss.str();
		this->errorMsg_ = ss.str();
		std::cout << ss.str() << std::endl;

		return status(statusSectionCode::ERR, (statusSubCodeType)e._error_code);
	}
	_MSDB_CATCH(const boost::exception& ex)
	{
		std::stringstream ss;
		ss << "* Error (Boost Exception) in query processing *\n" << std::endl;;
		ss << boost::diagnostic_information(ex) << std::endl;

		BOOST_LOG_TRIVIAL(error) << ss.str();
		this->errorMsg_ = ss.str();
		std::cout << ss.str() << std::endl;

		return status(statusSectionCode::ERR, (statusSubCodeType)statusErrCode::UNKNOWN);
	}
	_MSDB_CATCH_EXCEPTION(e)
	{
		std::stringstream ss;
		ss << "* Error (STD Exception) in query processing *\n" << e.what() << std::endl;;
		
		BOOST_LOG_TRIVIAL(error) << ss.str();
		this->errorMsg_ = ss.str();
		std::cout << ss.str() << std::endl;

		return status(statusSectionCode::ERR, (statusSubCodeType)statusErrCode::UNKNOWN);
	}
	_MSDB_CATCH_ALL
	{
		std::stringstream ss;
		ss << "* Error (Unknown Exception) in query processing *\n" << std::endl;;
		
		BOOST_LOG_TRIVIAL(error) << ss.str();
		this->errorMsg_ = ss.str();
		std::cout << ss.str() << std::endl;
		
		return status(statusSectionCode::ERR, (statusSubCodeType)statusErrCode::UNKNOWN);
	}
	_MSDB_CATCH_END
	return status(statusSectionCode::OK, (statusSubCodeType)statusOkCode::OK);
}

pTimer query::getTimer()
{
	return this->timer_;
}
pArrayDesc query::getArrayDesc()
{
	return this->arrDesc_;
}
void query::setDimBuffer(std::shared_ptr<std::vector<coor>> buffer)
{
	this->dimBuffer_ = buffer;
}
void query::setAttrBuffer(attributeId attrId, outBuffer buffer)
{
	if (arrDesc_ == nullptr)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_NO_ATTRIBUTE));
	}

	this->attrBuffers_[attrId] = buffer;
}
std::shared_ptr<std::vector<coor>> query::getDimBuffer()
{
	return this->dimBuffer_;
}
}	// core
}	// msdb