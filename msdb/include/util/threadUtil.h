#pragma once
#ifndef _MSDB_THREADUTIL_
#define _MSDB_THREADUTIL_

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


// https://stackoverflow.com/questions/57280865/best-way-to-wait-for-flag-in-thread
// @Miles Budnek
class ThreadFlag
{
public:
	ThreadFlag() : _flag{ false } {}

	inline void set()
	{
		std::lock_guard g(_mutex);
		_flag = true;
		_condVar.notify_all();
	}

	inline void clear()
	{
		std::lock_guard g(_mutex);
		_flag = false;
	}

	void wait()
	{
		std::unique_lock lock(_mutex);
		if(!_flag)
			_condVar.wait(lock, [this]() {return _flag; });
	}

	void wait_and_clear()
	{
		std::unique_lock lock(_mutex);
		if (!_flag)
			_condVar.wait(lock, [this]() {return _flag; });

		_flag = false;
	}
private:
	bool _flag;
	std::mutex _mutex;
	std::condition_variable _condVar;
};
}		// core
}		// msdb
#endif // _MSDB_THREADUTIL_


