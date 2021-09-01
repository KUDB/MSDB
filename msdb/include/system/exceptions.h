#pragma once
#ifndef _MSDB_EXCEPTIONS_H_
#define _MSDB_EXCEPTIONS_H_

#include <pch.h>
#include <system/errorCodes.h>

constexpr char const* const __NAMESPACE = "msdb";

namespace msdb
{
namespace core
{
// EXCEPTION MACROS
#define _MSDB_TRY_BEGIN     try {;

#define _MSDB_CATCH(x)      \
}                           \
catch (x) {;

#define _MSDB_CATCH_ALL     \
}                           \
catch (...) {;

#define _MSDB_CATCH_END     };


#define _MSDB_RETHROW   throw;
#define _MSDB_THROW(x)  throw x;

// MSDB FORMAT EXCEPTION
#define _MSDB_EXCEPTIONS(_error_category, _error_code)                \
    msdb::core::msdb_exception(__FILE__, __FUNCTION__, __LINE__,      \
                         __NAMESPACE,                           \
                         _error_category, _error_code,          \
                         #_error_category, #_error_code)

#define _MSDB_EXCEPTIONS_MSG(_error_category, _error_code, _msg)      \
    msdb::core::msdb_exception(__FILE__, __FUNCTION__, __LINE__,      \
                         __NAMESPACE,                           \
                         _error_category, _error_code,          \
                         #_error_category, #_error_code,        \
                         getErrorCategoryMsg(_error_category),  \
                         getErrorMsg(_error_code), _msg)
//////////////////////////////

    class msdb_exception : public virtual std::exception
    {
    public:
        msdb_exception() = default;

        msdb_exception(const char* file, const char* function, int32_t line,
                       const char* errors_namespace,
                       int32_t error_category, int32_t error_code,
                       const char* stringified_error_category,
                       const char* stringified_error_code);

        msdb_exception(const char* file, const char* function, int32_t line,
                       const char* errors_namespace,
                       int32_t error_category, int32_t error_code,
                       const char* stringified_error_category,
                       const char* stringified_error_code,
                       const char* error_category_msg,
                       const char* error_msg,
                       const char* what);

        virtual ~msdb_exception() noexcept = default;

    public:
        std::string _file;
        std::string _function;
        int32_t _line;
        std::string _errors_namespace;
        int32_t _error_category;
        int32_t _error_code;
        std::string _stringified_error_category;
        std::string _stringified_error_code;
        std::string _error_category_msg;
        std::string _error_msg;
    };
}		// core
}       // msdb
#endif