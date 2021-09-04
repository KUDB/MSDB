#include <pch.h>
#include <api_cpp/cpp_array.h>
#include <array/arrayMgr.h>

namespace msdb
{
Array::Array(Context context, core::arrayId arrId)
	: context_(context)
{
	this->desc_ = core::arrayMgr::instance()->getArrayDesc(arrId);
}

Array::Array(Context context, std::string arrName)
	: context_(context)
{
	this->desc_ = core::arrayMgr::instance()->getArrayDesc(
		core::arrayMgr::instance()->getArrayId(arrName));
}

ResultArray::ResultArray(Context context, core::pArray arr)
	: context_(context), arr_(arr)
{
}

void ResultArray::close()
{
	if (this->arr_ != nullptr)
	{
		this->arr_->flush();
		this->arr_ = nullptr;
	}
}
}		// msdb