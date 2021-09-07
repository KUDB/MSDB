#include <pch.h>
#include <xml/xmlFile.h>
#include <system/exceptions.h>

namespace msdb
{
namespace core
{
tinyxml2::XMLNode* xmlErrorHandler(tinyxml2::XMLNode* node)
{
	if (node == nullptr)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_IO_ERROR, MSDB_ER_XML_NO_NODE));
	}

	return node;
}

tinyxml2::XMLElement* xmlErrorHandler(tinyxml2::XMLElement* node)
{
	if (node == nullptr)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_IO_ERROR, MSDB_ER_XML_NO_ELEMENT));
	}

	return node;
}
const char* xmlErrorHandler(const char* text)
{
	if (text == nullptr)
	{
		_MSDB_THROW(_MSDB_EXCEPTIONS(MSDB_EC_IO_ERROR, MSDB_ER_XML_NO_ATTRIBUTE));
	}

	return text;
}
}		// core
}		// msdb