#include <pch.h>
#include <array/chunkFactory.h>

namespace msdb
{
namespace core
{
chunkFactory::chunkFactory()
{
}
chunkFactoryManager::~chunkFactoryManager()
{
	this->builders_.clear();
}
void chunkFactoryManager::registerFactoryBuilder(const std::string& name, const chunkFactoryBuilder& builder)
{
	this->builders_[name] = builder;
}

chunkFactoryBuilder chunkFactoryManager::getFactoryBuilder(const std::string& name)
{
	assert(this->builders_.find(name) != this->builders_.end());

	return this->builders_.at(name);
}
}		// core
}		// msdb