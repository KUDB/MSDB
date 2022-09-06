#pragma once
#ifndef _MSDB_OPERATORLIBRARY_H_
#define _MSDB_OPERATORLIBRARY_H_

#include <pch.h>
#include <util/singleton.h>
#include <query/opPlan.h>

#define REGISTER_BUILTIN_OPERATOR_FACTORY(strName, className)	\
	static msdb::core::builtInOperatorFactory<className> _builtInOperatorFactory##strName(#strName);

#define REGISTER_USER_DEFINED_OPERATOR_FACTORY(strName, className)	\
	static msdb::core::userDefinedOperatorFactory<className> userDefinedOperatorFactory##strName(#strName);

namespace msdb
{
namespace core
{
class operatorFactory;

/// <summary>
/// The singleton Library class holds the operatorFactories of entire operators used in this system.
/// 
/// The factories are the static object.
/// Library holds the pointer to the static objects
/// </summary>
class operatorLibrary : public singleton<operatorLibrary>
{
private:
	operatorLibrary();
	~operatorLibrary();

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="name"></param>
	/// <returns>
	/// True, if the operator is successfully registered, 
	/// False, if registration is failed
	/// </returns>
	bool registeOperator(std::string opName, operatorFactory* factory);
	pPlan retrieveOperator(std::string opName);

protected:
	friend singleton<operatorLibrary>;

private:
	std::map<std::string, operatorFactory*> factories_;
};

class operatorFactory
{
public:
	operatorFactory(const char* opName);

public:
	virtual pPlan requestOperatorPlan() = 0;

protected:
	std::string opName_;
};

template <typename Ty_>
class builtInOperatorFactory : public operatorFactory
{
public:
	builtInOperatorFactory(const char* opName)
		: operatorFactory(opName)
	{

	}
};

template <typename Ty_>
class userDefinedOperatorFactory : public operatorFactory
{
public:
	userDefinedOperatorFactory(const char* name)
		: operatorFactory(name)
	{
		
	}

	pPlan requestOperatorPlan() override
	{
		return std::make_shared<Ty_>();
	}
};
}		// core
}		// msdb
#endif	// _MSDB_OPERATORLIBRARY_H_