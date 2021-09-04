#pragma once
#ifndef _MSDB_CPP_ARRAY_H_
#define _MSDB_CPP_ARRAY_H_

#include <pch.h>
#include <array/array.h>
#include <query/query.h>
#include <api_cpp/cpp_context.h.>

namespace msdb
{
class Array
{
public:
	Array(Context context, core::arrayId arrId);
	Array(Context context, std::string arrName);

private:
	Context context_;
	core::pArrayDesc desc_;
};

class ResultArray
{
public:
	ResultArray(Context context, core::pArray arr, core::pQuery qry);

	std::shared_ptr<std::vector<core::coor>> getDimBuffer();
	template <typename Ty_>
	std::shared_ptr<std::vector<Ty_>> getAttrBuffer(core::attributeId attrId)
	{
		return std::static_pointer_cast<std::vector<Ty_>>(this->qry_->getAttrBuffer<Ty_>(attrId));
	}

public:
	void close();

private:
	Context context_;
	core::pArrayDesc arrDesc_;
	core::pQuery qry_;
};
}		// msdb
#endif	// _MSDB_CPP_ARRAY_H_