#include <pch.h>
#include <util/threadUtil.h>

namespace msdb
{
namespace core
{
threadUtil::threadUtil()
	: threadExist_(false), threadNums_(0)
{
}

threadUtil::~threadUtil()
{
}

void threadUtil::createThread(size_t threadNums)
{
	if (this->threadNums_ >= threadNums)
	{
		return;
	}

	this->io_service_ = std::make_shared<boost::asio::io_service>();
	this->work_ = std::make_shared<boost::asio::io_service::work>(*(this->io_service_));

	for (size_t i = 0; i < threadNums - this->threadNums_; ++i)
	{
		this->threadpool_.create_thread(boost::bind(&boost::asio::io_service::run, this->io_service_));
	}
	this->threadExist_ = true;
	this->threadNums_ = threadNums;
}
}		// core
}		// msdb