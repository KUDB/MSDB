#ifndef PCH_API_CPP_H
#define PCH_API_CPP_H

#include <encoding/encodingType.h>
#include <util/dataType.h>
#include <compression/materializedType.h>

#include <array/array.h>
#include <query/query.h>
#include <index/attributeIndex.h>

#ifdef API_CPP_EXPORTS
#define API_CPP_DLL_API __declspec(dllexport)
#else
#define API_CPP_DLL_API __declspec(dllimport)
#endif

#endif //PCH_API_CPP_H
