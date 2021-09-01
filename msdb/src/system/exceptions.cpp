#include <pch.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
    msdb_exception::msdb_exception(const char* file, const char* function, int32_t line,
                                   const char* errors_namespace,
                                   int32_t error_category, int32_t error_code,
                                   const char* stringified_error_category,
                                   const char* stringified_error_code)
        : _file(file)
        , _function(function)
        , _line(line)
        , _errors_namespace(errors_namespace)
        , _error_category(error_category)
        , _error_code(error_code)
        , _stringified_error_category(stringified_error_category)
        , _stringified_error_code(stringified_error_code)
        , _error_category_msg("")
        , _error_msg("")
    { }

    msdb_exception::msdb_exception(const char* file, const char* function, int32_t line,
                                   const char* errors_namespace,
                                   int32_t error_category, int32_t error_code,
                                   const char* stringified_error_category,
                                   const char* stringified_error_code,
                                   const char* error_category_msg,
                                   const char* error_msg,
                                   const char* what)
        : _file(file)
        , _function(function)
        , _line(line)
        , _errors_namespace(errors_namespace)
        , _error_category(error_category)
        , _error_code(error_code)
        , _stringified_error_category(stringified_error_category)
        , _stringified_error_code(stringified_error_code)
        , _error_category_msg(error_category_msg)
        , _error_msg(error_msg)
        , std::exception(what)
    { }
}		// core
}		// msdb