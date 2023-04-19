#pragma once
#ifndef _MSDB_MANAGERS_H_
#define _MSDB_MANAGERS_H_

#include <pch_manager.h>

#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <mutex>
#include <string>

namespace msdb
{
namespace core
{
class singletonHolder
{
public:
	singletonHolder() = default;
	~singletonHolder() {}

	static MANAGER_DLL_API void* get(const std::type_info& tInfo, void* obj)
	{
		if (getContainer()[tInfo.name()] != nullptr)
		{
			return getContainer()[tInfo.name()];
		}

		getLock().lock();
		if (getContainer()[tInfo.name()] == nullptr)
		{
			getContainer()[tInfo.name()] = obj;
		}
		getLock().unlock();

		return getContainer()[tInfo.name()];
	}

	MANAGER_DLL_API static std::unordered_map<std::string, void*>& getContainer();

private:
	static std::recursive_mutex& getLock();
};
}		// core
}		// msdb
#endif	// _MSDB_MANAGERS_H_