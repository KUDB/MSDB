#include <manager.h>

namespace msdb
{
namespace core
{
MANAGER_DLL_API std::unordered_map<std::string, void*>& singletonHolder::getContainer()
{
	static std::unordered_map<std::string, void*> _container;
	return _container;
}
std::recursive_mutex& singletonHolder::getLock()
{
	static std::recursive_mutex _lock;
	return _lock;
}
}	// core
}	// msdb