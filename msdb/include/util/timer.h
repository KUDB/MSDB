#pragma once
#ifndef _MSDB_TIMER_H_
#define _MSDB_TIMER_H_

#include <pch.h>

namespace msdb
{
namespace core
{
class timer;
using pTimer = std::shared_ptr<timer>;

enum class workType
{
	IDLE = 0,
	IO,
	COMPUTING,
	PARALLEL,
	LOGGING,
	ARRAY_CONSTRUCTING,
	COPY,
	OTHERS,
	TIMER_STOP
};

extern std::vector<const char*> strTimerWorkType;

class timerResult
{
public:
	std::map<size_t, float> thread;
	std::map<std::string, float> job;
	std::map<std::string, float> mainThreadJob;
	std::map<std::string, float> workType;
	std::map<std::string, float> jobWork;
};

class timer
{
public:
	timer();

public:
	void start(size_t threadId, const std::string& nextJobName, workType nextWorkType, const std::string& memo = "");
	void nextWork(size_t threadId, workType nextWorkType, const std::string& memo = "");
	void nextWork(size_t threadId, size_t parentThreadId, workType nextWorkType, const std::string& memo = "");
	void nextJob(size_t threadId, const std::string& nextJobName, workType nextWorkType, const std::string& memo = "");

	void pause(size_t threadId);
	void resume(size_t threadId, const std::string& nextJobName, workType nextWorkType);

	void printTime(bool printDetail = false);
	void recordTime(size_t expId, size_t trialId, size_t dataId, size_t methodId);
	size_t getNextJobId();
	size_t getMyJobId(size_t threadId);
	// TODO:: Job, Job name, getJobId

	double getExecutionTime();
	std::string getResult();
	std::string getDetailResult();



protected:
	void _start_NoLock_(size_t threadId, const std::string& nextJobName, workType nextWorkType, const std::string& memo = "");
	void _nextWork_NoLock_(size_t threadId, workType nextWorkType, const std::string& nextMemo = "");

private:
	typedef struct checkPoint
	{
		size_t threadId;
		std::chrono::duration<double> time_;
		size_t jobId;
		workType stype_;
		std::string memo_;
	}checkPoint;

private:
	std::chrono::system_clock::time_point time_;
	std::vector<checkPoint> records_;
	static std::mutex mutexJobId_;
	static std::mutex mutexJobUpdate_;
	size_t jobId_;
	std::map<size_t, workType> curJobType_;
	std::map<size_t, size_t> curJobIds_;
	std::map<size_t, std::chrono::system_clock::time_point> curJobTimes_;
	std::map<size_t, std::string> jobName_;
	std::map<size_t, std::string> curMemo_;
	
};
}		// core
}		// msdb
#endif // _MSDB_TIMER_H_
