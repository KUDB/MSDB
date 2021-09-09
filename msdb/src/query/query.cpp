#include <pch.h>
#include <query/query.h>
#include <query/queryMgr.h>
#include <query/opPlan.h>

namespace msdb
{
namespace core
{
query::query(std::shared_ptr<opPlan> qryPlan)
	: qryPlan_(qryPlan), timer_(std::make_shared<timer>()), arrDesc_(nullptr), dimBuffer_(std::make_shared<std::vector<coor>>())
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
		this->qryPlan_->process(shared_from_this());
	}
	_MSDB_CATCH_ALL
	{
		// TODO:: Log error
	}
	_MSDB_CATCH_END
	return status(statusSectionCode::OK, (statusSubCodeType)statusOkCode::OK);
}

pTimer query::getTimer()
{
	return this->timer_;
}
void query::setArrayDesc(pArrayDesc arrDesc)
{
	if(arrDesc_ == nullptr)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_QUERY_ERROR, MSDB_ER_NO_ARRAY));
	}

	this->arrDesc_ = arrDesc;
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