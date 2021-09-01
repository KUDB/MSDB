#include <pch.h>
#include <query/query.h>

namespace msdb
{
namespace core
{
query::query()
{
	this->timer_ = std::make_shared<timer>();
}
pTimer query::getTimer()
{
	return this->timer_;
}
}	// core
}	// msdb