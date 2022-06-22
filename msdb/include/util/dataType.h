#pragma once
#ifndef _MSDB_DATATYPE_H_
#define _MSDB_DATATYPE_H_

#include <pch.h>

template<class... Ts> struct visitHelper : Ts... { using Ts::operator()...; };
template<class... Ts> visitHelper(Ts...)->visitHelper<Ts...>; // line not needed in C++20...

namespace msdb
{
namespace core
{
class iTy
{
public:
	virtual std::string toString() = 0;
	virtual size_t size() = 0;
};

// TODO::Empty type?
template <typename Ty_>
class concreteTy : public iTy
{
public:
	virtual std::string toString() override
	{
		return typeid(Ty_).name();
	}
	virtual size_t size() override
	{
		return sizeof(Ty_);
	}

	// The returned string is exactly same with toString,
	// however this function is static.
	static std::string name()
	{
		return typeid(Ty_).name();
	}
};

using dataType = std::variant<
	concreteTy<bool>, concreteTy<char>,
	concreteTy<int8_t>, concreteTy<uint8_t>,
	concreteTy<int16_t>, concreteTy<uint16_t>,
	concreteTy<int32_t>, concreteTy<uint32_t>,
	concreteTy<int64_t>, concreteTy<uint64_t>,
	concreteTy<float>, concreteTy<double>>;

dataType string2dataType(std::string str);
}
}
#endif	// _MSDB_DATATYPE_H_