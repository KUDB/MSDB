#pragma once
#ifndef _MSDB_DIMENSION_ID_H_
#define _MSDB_DIMENSION_ID_H_

#include <pch.h>

namespace msdb
{
namespace core
{
typedef uint32_t dimensionId;

const dimensionId INVALID_DIMENSION_ID = static_cast<dimensionId>(~0);
}		// core
}		// msdb
#endif	// _MSDB_DIMENSION_ID_H_
