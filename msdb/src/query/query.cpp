#include <pch.h>
#include <query/query.h>
#include <query/queryMgr.h>

namespace msdb
{
namespace core
{
query::query(std::shared_ptr<opPlan> qryPlan)
	: qryPlan_(qryPlan), timer_(std::make_shared<timer>())
{

}

status query::submit()
{
	return queryMgr::instance()->querySubmit(shared_from_this());
}
status query::process()
{
	return status(statusSectionCode::OK, (statusSubCodeType)statusOkCode::OK);
}

pTimer query::getTimer()
{
	return this->timer_;
}
void query::setArrayDesc(pArrayDesc arrDesc)
{
	if(arrDesc_ != nullptr)
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