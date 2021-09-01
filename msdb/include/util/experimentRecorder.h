#pragma once
#ifndef _MSDB_EXPERIMENT_RECORDER
#define _MSDB_EXPERIMENT_RECORDER

#include <pch.h>
#include <util/singleton.h>

namespace msdb
{
namespace core
{
class experimentRecorder;
using pRecorder = std::shared_ptr<experimentRecorder>;

class experimentRecorder : public singleton<experimentRecorder>
{
public:
	experimentRecorder();
	~experimentRecorder();

private:
	typedef struct record
	{
		size_t experimentId;
		size_t trialId;
		size_t dataId;
		size_t opId;
		std::string opName;
		float procTime;
		size_t methodId;
	};

public:
	void insert(size_t experimentId, size_t trialId, size_t dataId, size_t jobId, std::string jobName, float procTime, size_t methodId = 0);
	void print();
	void printLine(size_t tag, std::map<size_t, record>& line);

private:
	std::vector<record> records_;
	std::set<size_t> experimentIds_;
	std::set<size_t> dataIds_;
};
}		// core
}		// msdb
#endif	// _MSDB_EXPERIMENT_RECORDER