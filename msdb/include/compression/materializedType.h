#pragma once
#ifndef _MSDB_MATERIALIZEDTYPE_H_
#define _MSDB_MATERIALIZEDTYPE_H_

#include <pch.h>
#include <util/enumType.h>

namespace msdb
{
namespace core
{
ENUM_MACRO(materializedType,
		   FLATTEN, NESTED);
}		// core
}		// msdb
#endif	// _MSDB_MATERIALIZEDTYPE_H_