#include <pch.h>
#include <system/exceptions.h>

namespace msdb
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
    , std::exception(stringified_error_code)
{
    this->_whatDetail = this->makeDetailWhat();
}

msdb_exception::msdb_exception(const char* file, const char* function, int32_t line, 
                                const char* errors_namespace, int32_t error_category, int32_t error_code,
                                const char* stringified_error_category, const char* stringified_error_code, 
                                const char* error_category_msg, const char* error_msg, 
                                const std::string what)
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
    , _what(what)
    , std::exception(_what.c_str())
{ 
    this->_whatDetail = this->makeDetailWhat();
}

std::string msdb_exception::makeDetailWhat() const
{
    std::stringstream str;
    str << this->_what << "\n";
    str << "Error" << this->_error_category << "(" << this->_stringified_error_category << "): " << this->_error_category_msg << "\n";
    str << "Code" << this->_error_code << "(" << this->_stringified_error_code << "): " << this->_error_msg << "\n";
    str << "[file: " << this->_file << ", line: " << this->_line << "]\n";
    str << "[function: " << this->_function << "]";
    return str.str();
}

_NODISCARD const char* msdb_exception::what() const
{
    return this->_whatDetail.c_str();
}
}		// msdb