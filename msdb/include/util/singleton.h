#pragma once
#ifndef _MSDB_SINGLETON_H_
#define _MSDB_SINGLETON_H_

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
		static _derived instance;
		return &instance;
	}

	// Delete copy and move constructor
	singleton(const singleton<_derived>&) = delete;
	singleton(singleton&&) = delete;

	singleton<_derived>& operator=(const singleton<_derived>&) = delete;
	singleton<_derived>& operator=(singleton<_derived>&&) = delete;

protected:

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