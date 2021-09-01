#pragma once
#ifndef _MSDB_CONFIG_H_
#define _MSDB_CONFIG_H_

#include <pch.h>
#include <xml/tinyxml2.h>

namespace msdb
{
namespace core
{
#define _MSDB_XML_ELE_ID				"id"
#define _MSDB_XML_ELE_NAME				"name"
#define _MSDB_XML_ELE_SIZE				"size"
#define _MSDB_XML_ELE_TYPE				"type"
#define _MSDB_XML_ELE_START				"start"
#define _MSDB_XML_ELE_END				"end"

class childExploreJob;

enum class ConfigType
{
	ARRAYLIST,
	ARRAY,
	DIMENSION,
	ATTRIBUTE,
	SYSTEM
};

class config
{
public:
	config(tinyxml2::XMLNode* root);

	virtual ConfigType getType() = 0;
	void save(const char* path);

protected:
	void xmlChildExplore(tinyxml2::XMLNode* root, void* list, const childExploreJob& job);
	void xmlChildExplore(tinyxml2::XMLNode* root, void* list, void (*f)(tinyxml2::XMLNode*, void*));


protected:
	tinyxml2::XMLNode* _root;
};

class childExploreJob
{
public:
	childExploreJob() {};

public:
	virtual void action(tinyxml2::XMLNode* root, void* output) const = 0;
};
}		// core
}		// msdb
#endif	// _MSDB_CONFIG_H_
