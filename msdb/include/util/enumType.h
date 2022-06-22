#pragma once
#ifndef _MSDB_ENUMTYPE_H_
#define _MSDB_ENUMTYPE_H_

#include <pch.h>
#include <boost/preprocessor.hpp>

namespace msdb
{
namespace core
{
#define PROCESS_ONE_ELEMENT(r, unused, idx, elem) \
  BOOST_PP_COMMA_IF(idx) BOOST_PP_STRINGIZE(elem)

#define ENUM_MACRO(name, ...)\
    enum class name { __VA_ARGS__ };\
    static const char *name##Strings[] = { BOOST_PP_SEQ_FOR_EACH_I(PROCESS_ONE_ELEMENT, %%, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)) };\
    template<typename T>\
    constexpr const char *name##ToString(T value) { return name##Strings[static_cast<int>(value)]; }
}		// core
}		// msdb
#endif	// _MSDB_ENUMTYPE_H_
