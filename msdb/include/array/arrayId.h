#pragma once
#ifndef _MSDB_ARRAY_ID_H_
#define _MSDB_ARRAY_ID_H_

#include <cstdint>

namespace msdb
{
namespace core
{
typedef uint32_t arrayId;

const arrayId INVALID_ARRAY_ID = static_cast<arrayId>(~0);
}		// core
}		// msdb
#endif	// _MSDB_ARRAY_ID_H_