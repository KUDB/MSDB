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

public:
	inline core::pArrayDesc getDesc()
	{
		return this->arrDesc_;
	}

private:
	Context context_;
	core::pArrayDesc arrDesc_;
};

class ResultArray
{
public:
	ResultArray(Context context, core::pQuery qry);

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

class Dimension
{
public:
	Dimension(std::string name, 
			  uint64_t start, uint64_t end, 
			  uint64_t chunkSize, uint64_t blockSize);

public:
	std::shared_ptr<core::dimensionDesc> getDesc();

private:
	std::shared_ptr<core::dimensionDesc> dimDesc_;
};

class Attribute
{
public:
	Attribute(std::string name, core::eleType type);

public:
	std::shared_ptr<core::attributeDesc> getDesc();

	operator core::attributeDesc();

private:
	std::shared_ptr<core::attributeDesc> attrDesc_;
};
}		// msdb
#endif	// _MSDB_CPP_ARRAY_H_