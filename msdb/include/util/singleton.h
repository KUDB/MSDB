#pragma once
#ifndef _MSDB_SINGLETON_H_
#define _MSDB_SINGLETON_H_

#include <typeinfo>
#include <manager.h>

namespace msdb 
{
namespace core
{
template <typename _derived>
class singleton
{
public:
	static _derived* instance()
	{
		static _derived *instance = nullptr;

		if (instance == nullptr)
		{
			BOOST_LOG_TRIVIAL(trace) << "Singleton::Try Regi: " << __FUNCTION__;
			_derived* temp = new _derived();
			// singletonHolder::get is thread-safe
			instance = (_derived*)singletonHolder::get(typeid(_derived), temp);
			if (instance != temp)
			{
				BOOST_LOG_TRIVIAL(trace) << "Singleton::Regi Fail: " << __FUNCTION__ << instance;
				delete temp;
			}
			else
			{
				BOOST_LOG_TRIVIAL(trace) << "Singleton::Regi Success: " << __FUNCTION__ << instance;
			}
		}
		BOOST_LOG_TRIVIAL(trace) << "Singleton::" << __FUNCTION__ << ": " << instance;
		return instance;
	}

	// Delete copy and move constructor
	singleton(const singleton<_derived>&) = delete;
	singleton(singleton&&) = delete;

	singleton<_derived>& operator=(const singleton<_derived>&) = delete;
	singleton<_derived>& operator=(singleton<_derived>&&) = delete;

protected:
	singleton() : isInit(false)
	{
		this->isInit = true;
	}
	virtual ~singleton() {}

protected:
	bool isInit;
};
}		// core
}		// msdb
#endif