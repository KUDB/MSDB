#include <pch.h>
#include <array/arrayMgr.h>
#include <query/query.h>
#include <api_cpp/cpp_array.h>

namespace msdb
{
Array::Array(Context context, core::arrayId arrId)
	: context_(context)
{
	this->arrDesc_ = core::arrayMgr::instance()->getArrayDesc(arrId);
}

Array::Array(Context context, std::string arrName)
	: context_(context)
{
	this->arrDesc_ = core::arrayMgr::instance()->getArrayDesc(
		core::arrayMgr::instance()->getArrayId(arrName));
}

ResultArray::ResultArray(Context context, core::pQuery qry)
	: context_(context), arrDesc_(qry->getArrayDesc()), qry_(qry)
{
}

std::shared_ptr<std::vector<core::coor>> ResultArray::getDimBuffer()
{
	return this->qry_->getDimBuffer();
}

void ResultArray::close()
{
	this->arrDesc_ = nullptr;
	this->qry_ = nullptr;
}
}		// msdb