#pragma once
#ifndef _MSDB_THREADUTIL_
#define _MSDB_THREADUTIL_

#include <pch.h>

using namespace std::chrono_literals;

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

	bool wait()
	{
		std::unique_lock lock(_mutex);
		if(!_flag)
			_condVar.wait(lock, [this]() {return _flag; });

		return true;
	}

	bool wait_and_clear()
	{
		wait();

		std::unique_lock lock(_mutex);
		_flag = false;

		return true;
	}

	bool wait(const std::chrono::milliseconds duration)
	{
		std::unique_lock lock(_mutex);

		// 'false' if the predicate stop_waiting still evaluates to false after the rel_time timeout expired, otherwise 'true'.
		return _condVar.wait_for(lock, duration, [this] {return _flag; });
	}

	bool wait_and_clear(const std::chrono::milliseconds duration)
	{
		auto status = wait(duration);

		if (!status)
		{
			return false;
		}
		std::unique_lock lock(_mutex);
		_flag = false;

		return true;
	}

	bool wait(const int milliseconds)
	{
		return wait(std::chrono::milliseconds(milliseconds));
	}

	bool wait_and_clear(const int milliseconds)
	{
		return wait_and_clear(std::chrono::milliseconds(milliseconds));
	}

private:
	bool _flag;
	std::mutex _mutex;
	std::condition_variable _condVar;
};
}		// core
}		// msdb
#endif // _MSDB_THREADUTIL_


