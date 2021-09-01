#include <pch.h>
#include <system/errorCodes.h>

namespace msdb
{
namespace core
{
	const char* getErrorCategoryMsg(int32_t code)
	{
		switch (code)
		{
#define X(_name, _code, _msg)	case _code: \
			return #_msg;	
#include <system/errorCategories.inc>
#undef X
		}
	}

	const char* getErrorMsg(int32_t code)
	{
		switch (code)
		{
#define X(_name, _code, _msg)	case _code: \
			return #_msg;	
#include <system/errors.inc>
#undef X
		}
	}
}		// core
}		// msdb