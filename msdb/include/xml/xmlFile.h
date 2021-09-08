#pragma once
#ifndef _MSDB_XMLFILE_H_
#define _MSDB_XMLFILE_H_

#include <pch.h>
#include <xml/tinyxml2.h>

namespace msdb
{
namespace core
{
tinyxml2::XMLNode* xmlErrorHandler(tinyxml2::XMLNode* node);
tinyxml2::XMLElement* xmlErrorHandler(tinyxml2::XMLElement* node);
const char* xmlErrorHandler(const char* text);
}		// core
}		// msdb
#endif	// _MSDB_XMLFILE_H_