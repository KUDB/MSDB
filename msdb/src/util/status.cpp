#include <pch.h>
#include <util/status.h>

namespace msdb
{
namespace core
{
status::status(statusSectionCode code, statusSubCodeType subCode)
	: sectionCode_(code), subCode_(subCode)
{
}
}		// core
}		// msdb