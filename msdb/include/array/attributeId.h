#pragma once
#ifndef _MSDB_ATTRIBUTE_ID_H_
#define _MSDB_ATTRIBUTE_ID_H_

#include <cstdint>

namespace msdb
{
namespace core
{
typedef uint32_t attributeId;

const attributeId INVALID_ATTRIBUTE_ID = static_cast<attributeId>(~0);
}		// core
}		// msdb
#endif	// _MSDB_ATTRIBUTE_ID_H_
