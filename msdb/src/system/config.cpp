#include <pch.h>
#include <system/config.h>
#include <xml/tinyxml2.h>

namespace msdb
{
namespace core
{
config::config(tinyxml2::XMLNode* root)
{
	this->_root = root;
}

void config::save(const char* path)
{
	this->_root->ToDocument()->SaveFile(path);
}
//void config::xmlChildExplore(tinyxml2::XMLNode* root, void* list, void(config::*f)(tinyxml2::XMLNode*, void*))
void config::xmlChildExplore(tinyxml2::XMLNode* root, void* list, const childExploreJob& job)
{
	for (auto child = (tinyxml2::XMLNode*)root->FirstChild();
		 child != 0;
		 child = (tinyxml2::XMLNode*)child->NextSibling())
	{
		job.action(child, list);
	}
}
void config::xmlChildExplore(tinyxml2::XMLNode* root, void* list, void(*f)(tinyxml2::XMLNode*, void*))
{
	for (auto child = (tinyxml2::XMLNode*)root->FirstChild();
		 child != 0;
		 child = (tinyxml2::XMLNode*)child->NextSibling())
	{
		(*f)(child, list);
	}
}
}		// core
}		// msdb
