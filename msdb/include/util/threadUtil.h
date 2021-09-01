#pragma once
#ifndef _MSDB_THREADUTIL_
#define _MSDB_THREADUTIL_

#define _MSDB_ACTION_THREAD_NUM_	8

#include <pch.h>

namespace msdb
{
namespace core
{
inline unsigned long getThreadId()
{
	return atol(boost::lexical_cast<std::string>(boost::this_thread::get_id()).c_str());
}

class threadUtil
{
public:
	threadUtil();
	~threadUtil();

public:
	void createThread(size_t threadNums);

	inline void threadStop()
	{
		this->io_service_->stop();
	}
	inline void threadJoin()
	{
		this->threadpool_.join_all();
	}
	inline std::shared_ptr<boost::asio::io_service> get_io_service()
	{
		return this->io_service_;
	}

private:
	bool threadExist_;
	size_t threadNums_;
	std::shared_ptr<boost::asio::io_service> io_service_;
	std::shared_ptr<boost::asio::io_service::work> work_;
	boost::thread_group threadpool_;
};
}		// core
}		// msdb
#endif // _MSDB_THREADUTIL_


