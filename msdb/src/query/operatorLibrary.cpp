#include <pch.h>
#include <query/operatorLibrary.h>

namespace msdb
{
namespace core
{
operatorLibrary::operatorLibrary()
{
}
operatorFactory::operatorFactory(const char* opName)
{
	this->opName_ = opName;
	operatorLibrary::instance()->registeOperator(this->opName_, this);
}
operatorLibrary::~operatorLibrary()
{
}
bool operatorLibrary::registeOperator(std::string opName, operatorFactory* factory)
{
	if (this->factories_.find(opName) != this->factories_.end())
	{
		// Duplicated operator name
		BOOST_LOG_TRIVIAL(error) << "Duplicated operator name: " << opName;
		return false;
	}

	this->factories_[opName] = factory;
}
pPlan operatorLibrary::retrieveOperator(std::string opName)
{
	if (this->factories_.find(opName) == this->factories_.end())
	{
		std::stringstream ss;
		ss << "Cannot retrieve operator name: " << opName << std::endl;
		ss << "Cannot find the operator in the operatorLibrary";
		_MSDB_THROW(_MSDB_EXCEPTIONS_MSG(MSDB_EC_QUERY_ERROR, MSDB_ER_CANNOT_FIND, ss.str()));
	}
	operatorFactory* factory = this->factories_.at(opName);
	return factory->requestOperatorPlan();
}
}		// core
}		// msdb