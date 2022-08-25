#include <pch.h>
#include <util/timer.h>
#include <util/logger.h>
#include <util/experimentRecorder.h>

namespace msdb
{
namespace core
{
extern std::vector<const char*> strTimerWorkType = {
	"IDLE", "IO", "COMPUTING", "PARALLEL", "LOGGING", "ARRAY_CONSTRUCTING", "COPY", "OTHERS", "TIMER_STOP"
};

timer::timer()
	: jobId_(0)
{
}

void timer::start(size_t threadId, const std::string& nextJobName, workType nextWorkType, const std::string& memo)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	this->_start_NoLock_(threadId, nextJobName, nextWorkType, memo);
	//////////////////////////////
}

void timer::nextWork(size_t threadId, workType nextWorkType, const std::string& memo)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	this->_nextWork_NoLock_(threadId, nextWorkType, memo);
	//////////////////////////////
}

void timer::nextWork(size_t threadId, size_t parentThreadId, workType nextWorkType, const std::string& memo)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	if(curJobIds_.find(threadId) == curJobIds_.end())
	{
		// Start new thread work
		auto curJobId = this->getNextJobId();
		this->curJobIds_[threadId] = curJobId;
		this->curJobType_[threadId] = nextWorkType;
		this->jobName_[curJobId] = this->jobName_[parentThreadId];
		this->curJobTimes_[threadId] = std::chrono::system_clock::now();
		this->curMemo_[threadId] = memo;
	}else
	{
		this->_nextWork_NoLock_(threadId, nextWorkType, memo);
	}

	//////////////////////////////
}

void timer::_start_NoLock_(size_t threadId, const std::string& nextJobName, workType nextWorkType, const std::string& memo)
{
	auto curJobId = this->getNextJobId();
	this->curJobIds_[threadId] = curJobId;
	this->curJobType_[threadId] = nextWorkType;
	this->jobName_[curJobId] = nextJobName;
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
	this->curMemo_[threadId] = memo;
}

void timer::_nextWork_NoLock_(size_t threadId, workType nextWorkType, const std::string& nextMemo)
{
	std::chrono::duration<double> record = std::chrono::system_clock::now() - this->curJobTimes_[threadId];
	this->records_.push_back({ threadId, record, this->curJobIds_[threadId], this->curJobType_[threadId], this->curMemo_[threadId] });
	this->curJobTimes_[threadId] = std::chrono::system_clock::now();
	this->curJobType_[threadId] = nextWorkType;
	this->curMemo_[threadId] = nextMemo;
}

void timer::nextJob(size_t threadId, const std::string& nextJobName, workType nextWorkType, const std::string& memo)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	if (curJobIds_.find(threadId) == curJobIds_.end())
	{
		this->_start_NoLock_(threadId, nextJobName, nextWorkType, memo);
	}else
	{
		this->_nextWork_NoLock_(threadId, nextWorkType, memo);

	}

	auto curJobId = this->getNextJobId();
	this->curJobIds_[threadId] = curJobId;
	this->jobName_[curJobId] = nextJobName;
	//////////////////////////////
}

void timer::pause(size_t threadId)
{
	this->nextJob(threadId, "pause", workType::TIMER_STOP);
}

void timer::resume(size_t threadId, const std::string& nextJobName, workType nextJobType)
{
	this->nextJob(threadId, nextJobName, nextJobType);
}

void timer::printTime(bool printDetail)
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	std::map<size_t, float> thread;
	std::map<std::string, float> job;
	std::map<std::string, float> mainThreadJob;
	std::map<std::string, float> workType;
	std::map<std::string, float> jobWork;
	std::vector<std::tuple<std::string, float>> ioWorks;

	for (int i = 0; i < this->records_.size(); i++)
	{
		if(printDetail)
		{
			BOOST_LOG_TRIVIAL(info) <<
				this->records_[i].threadId << "\t" <<
				this->records_[i].jobId << "\t" <<
				boost::str(boost::format("%f") % this->records_[i].time_.count()) << "\t" <<
				"[" << jobName_[this->records_[i].jobId] << " / " <<
				strTimerWorkType[static_cast<int>(this->records_[i].stype_)] << "]";
		}
		
		if (thread.find(this->records_[i].threadId) != thread.end())
		{
			thread.find(this->records_[i].threadId)->second += this->records_[i].time_.count();
		} else
		{
			thread.insert(std::make_pair(this->records_[i].threadId, this->records_[i].time_.count()));
		}

		if (job.find(jobName_[this->records_[i].jobId]) != job.end())
		{
			job.find(jobName_[this->records_[i].jobId])->second += this->records_[i].time_.count();
		} else
		{
			job.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i].time_.count()));
		}

		if(this->records_[i].threadId == 0)
		{
			if (mainThreadJob.find(jobName_[this->records_[i].jobId]) != mainThreadJob.end())
			{
				mainThreadJob.find(jobName_[this->records_[i].jobId])->second += this->records_[i].time_.count();
			} else
			{
				mainThreadJob.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i].time_.count()));
			}
		}

		if (workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != workType.end())
		{
			workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		} else
		{
			workType.insert(std::make_pair(strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}

		// To analysis IO works
		if (this->records_[i].stype_ == workType::IO)
		{
			ioWorks.push_back({ this->records_[i].memo_, this->records_[i].time_.count() });
		}

		if (jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != jobWork.end())
		{
			jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		} else
		{
			jobWork.insert(std::make_pair(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====threadId=====";
	for (auto it = thread.begin(); it != thread.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====jobName=====";
	for (auto it = job.begin(); it != job.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====MainThreadJobName=====";
	for (auto it = mainThreadJob.begin(); it != mainThreadJob.end(); it++)
	{
		BOOST_LOG_TRIVIAL(info) <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====WorkType=====";
	for (auto it = workType.begin(); it != workType.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]";
	}

	BOOST_LOG_TRIVIAL(info) << '\n';
	BOOST_LOG_TRIVIAL(info) << "=====jobName, WorkType=====";
	for (auto it = jobWork.begin(); it != jobWork.end(); it++) {
		BOOST_LOG_TRIVIAL(info) <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]";
	}
	//////////////////////////////

	//////////////////////////////
	// IO Works Results
	if (ioWorks.size())
	{
		double max = -1, min = 1, sum = 0;
		int iMax = 0, iMin = 0;
		for (int i = 0; i < ioWorks.size(); ++i)
		{
			auto t = ioWorks[i];
			double value = std::get<1>(t);
			if (value > max)
			{
				max = value;
				iMax = i;
			}
			if (value < min)
			{
				min = value;
				iMin = i;
			}

			sum += value;
		}
		double avg = sum * 1.0 / ioWorks.size();

		BOOST_LOG_TRIVIAL(info) << "=====IO Works=====" << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Max (" << iMax << "): " << max << "/" << std::get<0>(ioWorks[iMax]) << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Min (" << iMin << "): " << min << "/" << std::get<0>(ioWorks[iMin]) << std::endl;
		BOOST_LOG_TRIVIAL(info) << "Avg: " << avg << "(size: " << ioWorks.size() << ")" << std::endl;
	}
	//////////////////////////////
}

void timer::recordTime(size_t expId, size_t trialId, size_t dataId, size_t methodId)
{
	auto expRecorder = experimentRecorder::instance();

	std::map<std::string, float> mainThreadJob;
	std::map<std::string, checkPoint> mainThreadJobInfo;

	for (int i = 0; i < this->records_.size(); i++)
	{
		if (this->records_[i].threadId == 0)
		{
			if (mainThreadJob.find(jobName_[this->records_[i].jobId]) != mainThreadJob.end())
			{
				mainThreadJob.find(jobName_[this->records_[i].jobId])->second += this->records_[i].time_.count();
			} else
			{
				mainThreadJob.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i].time_.count()));
				mainThreadJobInfo.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i]));
			}
		}
	}
	
	for(auto j : mainThreadJobInfo)
	{
		expRecorder->insert(expId, trialId, dataId, j.second.jobId, jobName_[j.second.jobId], mainThreadJob[j.first], methodId);
	}
}

size_t timer::getNextJobId()
{
	std::lock_guard<std::mutex> guard(this->mutexJobId_);

	size_t outJobId = this->jobId_++;

	return outJobId;
}
size_t timer::getMyJobId(size_t threadId)
{
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	if(curJobIds_.find(threadId) != curJobIds_.end())
	{
		return this->curJobIds_[threadId];
	}

	return 0;
}

double timer::getExecutionTime()
{
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);
	std::map<size_t, float> thread;
	for (int i = 0; i < this->records_.size(); i++)
	{
		if (thread.find(this->records_[i].threadId) != thread.end())
		{
			thread.find(this->records_[i].threadId)->second += this->records_[i].time_.count();
		}
		else
		{
			thread.insert(std::make_pair(this->records_[i].threadId, this->records_[i].time_.count()));
		}
	}

	if (thread.empty())
	{
		return 0.0;
	}
	//////////////////////////////

	return thread[0];
}

std::string timer::getResult()
{
	return std::string();
}

std::string timer::getDetailResult()
{
	std::stringstream ss;
	ss << "########################################" << std::endl;
	ss << "# Query execution time report" << std::endl;
	ss << "#" << std::endl;
	//////////////////////////////
	// LOCK
	std::lock_guard<std::mutex> guard(this->mutexJobUpdate_);

	std::map<size_t, float> thread;
	std::map<std::string, float> job;
	std::map<std::string, float> mainThreadJob;
	std::map<std::string, float> workType;
	std::map<std::string, float> jobWork;
	std::vector<std::tuple<std::string, float>> ioWorks;

	bool printDetail = false;

	if (printDetail)
	{
		ss << "+------+-------+-------+------------------------+" << std::endl;
		ss << " Thread\tJob Id\tTime\t[Job name / Work type]" << "" << std::endl;
		ss << "+------+-------+-------+------------------------+" << std::endl;
	}
	for (int i = 0; i < this->records_.size(); i++)
	{
		if (printDetail)
		{
			ss << " " << 
				this->records_[i].threadId << "\t" <<
				this->records_[i].jobId << "\t" <<
				boost::str(boost::format("%1$.5f") % this->records_[i].time_.count()) << "\t" <<
				"[" << jobName_[this->records_[i].jobId] << " / " <<
				strTimerWorkType[static_cast<int>(this->records_[i].stype_)] << "]" << std::endl;
		}

		if (thread.find(this->records_[i].threadId) != thread.end())
		{
			thread.find(this->records_[i].threadId)->second += this->records_[i].time_.count();
		}
		else
		{
			thread.insert(std::make_pair(this->records_[i].threadId, this->records_[i].time_.count()));
		}

		if (job.find(jobName_[this->records_[i].jobId]) != job.end())
		{
			job.find(jobName_[this->records_[i].jobId])->second += this->records_[i].time_.count();
		}
		else
		{
			job.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i].time_.count()));
		}

		if (this->records_[i].threadId == 0)
		{
			if (mainThreadJob.find(jobName_[this->records_[i].jobId]) != mainThreadJob.end())
			{
				mainThreadJob.find(jobName_[this->records_[i].jobId])->second += this->records_[i].time_.count();
			}
			else
			{
				mainThreadJob.insert(std::make_pair(jobName_[this->records_[i].jobId], this->records_[i].time_.count()));
			}
		}

		if (workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != workType.end())
		{
			workType.find(strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		}
		else
		{
			workType.insert(std::make_pair(strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}

		// To analysis IO works
		if (this->records_[i].stype_ == workType::IO)
		{
			ioWorks.push_back({ this->records_[i].memo_, this->records_[i].time_.count() });
		}

		if (jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)]) != jobWork.end())
		{
			jobWork.find(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)])->second += this->records_[i].time_.count();
		}
		else
		{
			jobWork.insert(std::make_pair(jobName_[this->records_[i].jobId] + " / " + strTimerWorkType[static_cast<int>(this->records_[i].stype_)], this->records_[i].time_.count()));
		}
	}

	if (printDetail)
	{
		ss << "+------+-------+-------+------------------------+" << std::endl;
	}

	ss << std::endl;
	ss << "=====threadId=====" << std::endl;
	for (auto it = thread.begin(); it != thread.end(); it++) {
		ss <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]" << std::endl;
	}

	ss << std::endl;
	ss << "=====jobName=====" << std::endl;
	for (auto it = job.begin(); it != job.end(); it++) {
		ss <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]" << std::endl;
	}

	ss << std::endl;
	ss << "=====MainThreadJobName=====" << std::endl;
	for (auto it = mainThreadJob.begin(); it != mainThreadJob.end(); it++)
	{
		ss <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]" << std::endl;
	}

	ss << std::endl;
	ss << "=====WorkType=====" << std::endl;
	for (auto it = workType.begin(); it != workType.end(); it++) {
		ss <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]" << std::endl;
	}

	ss << std::endl;
	ss << "=====jobName, WorkType=====" << std::endl;
	for (auto it = jobWork.begin(); it != jobWork.end(); it++) {
		ss <<
			boost::format("%1$.5f") % it->second << " [" <<
			it->first << "]" << std::endl;
	}
	//////////////////////////////

	//////////////////////////////
	// IO Works Results
	if (ioWorks.size())
	{
		double max = -1, min = 1, sum = 0;
		int iMax = 0, iMin = 0;
		for (int i = 0; i < ioWorks.size(); ++i)
		{
			auto t = ioWorks[i];
			double value = std::get<1>(t);
			if (value > max)
			{
				max = value;
				iMax = i;
			}
			if (value < min)
			{
				min = value;
				iMin = i;
			}

			sum += value;
		}
		double avg = sum * 1.0 / ioWorks.size();

		ss << "=====IO Works=====" << std::endl;
		ss << "Max (" << iMax << "): " << max << "/" << std::get<0>(ioWorks[iMax]) << std::endl;
		ss << "Min (" << iMin << "): " << min << "/" << std::get<0>(ioWorks[iMin]) << std::endl;
		ss << "Avg: " << avg << "(size: " << ioWorks.size() << ")" << std::endl;
	}
	//////////////////////////////

	return ss.str();
}

std::mutex timer::mutexJobId_;
std::mutex timer::mutexJobUpdate_;

}		// core
}		// msdb