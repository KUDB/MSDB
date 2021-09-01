#include <pch.h>
#include <util/experimentRecorder.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
experimentRecorder::experimentRecorder()
{

}

experimentRecorder::~experimentRecorder()
{
	this->print();
}

void experimentRecorder::insert(size_t experimentId, size_t trialId, size_t dataId, size_t opId, std::string opName, float procTime, size_t methodId)
{
	this->experimentIds_.insert(experimentId);
	this->dataIds_.insert(dataId);

	this->records_.push_back(record{ experimentId, trialId, dataId, opId, opName, procTime, methodId });
}

void experimentRecorder::print()
{
	auto expIdIt = this->experimentIds_.begin();
	while(expIdIt != this->experimentIds_.end())
	{
		BOOST_LOG_TRIVIAL(info) << "##############################";
		BOOST_LOG_TRIVIAL(info) << "EXPERIMENT";
		BOOST_LOG_TRIVIAL(info) << "##############################";

		// Collect Same Experiment data / trial
		size_t curExpId = *expIdIt;

		if(curExpId == 100)
		{
			// Range query
			std::map<size_t, std::vector<std::vector<record>>> curExpDataRecords;

			// Combine records with dataID
			for(auto r : this->records_)
			{
				if(r.experimentId == curExpId)
				{
					if (curExpDataRecords.find(r.dataId) == curExpDataRecords.end())
					{
						curExpDataRecords[r.dataId] = std::vector<std::vector<record>>();
					}

					assert(r.methodId < 5);
					while(curExpDataRecords[r.dataId].size() <= r.methodId)
					{
						curExpDataRecords[r.dataId].push_back(std::vector<record>());
					}

					curExpDataRecords[r.dataId][r.methodId].push_back(r);
				}
			}

			for(auto curDataRecords : curExpDataRecords)
			{
				if (curDataRecords.second.size() == 0)
					continue;

				if (curDataRecords.second[0].size() == 0)
					continue;

				size_t curDataId = curDataRecords.second[0][0].dataId;
				size_t curMethodId = curDataRecords.second[0][0].methodId;

				BOOST_LOG_TRIVIAL(info) << "------------------------------";
				BOOST_LOG_TRIVIAL(info) << "CUR Data: " << curDataId << curMethodId; 
				BOOST_LOG_TRIVIAL(info) << "------------------------------";

				for(auto curMethodRecords : curDataRecords.second)
				{
					std::vector<std::vector<record>> trialRecords;

					for(auto r : curMethodRecords)
					{
						assert(r.trialId < 10);
						
						while (trialRecords.size() <= r.trialId)
						{
							trialRecords.push_back(std::vector<record>());
						}
						trialRecords[r.trialId].push_back(r);
					}

					for (auto curTrialRecords : trialRecords)
					{
						std::map<size_t, record> line;
						for (auto r : curTrialRecords)
						{
							line[r.opId] = r;
						}

						if(line.size())
						{
							this->printLine(line.begin()->second.trialId, line);
						}
					}
				}
			}
		}else
		{
			std::vector<std::vector<record>> curExpRecords;

			// Combine records with trialID
			for (auto r : this->records_)
			{
				if (r.experimentId == curExpId)
				{
					assert(r.trialId < 10);

					while (curExpRecords.size() <= r.trialId)
					{
						curExpRecords.push_back(std::vector<record>());
					}

					curExpRecords[r.trialId].push_back(r);
				}
			}

			// Rest experiments
			// Same Exp, Same Trial
			int curTrial = 0;
			for (auto curTrialRecords : curExpRecords)
			{
				BOOST_LOG_TRIVIAL(info) << "------------------------------";
				BOOST_LOG_TRIVIAL(info) << "CUR TRIAL: " << curTrial;
				BOOST_LOG_TRIVIAL(info) << "------------------------------";

				// Collect Same Data
				std::map<size_t, std::vector<std::vector<record>>> trialRecords;

				auto dataIdIt = this->dataIds_.begin();
				while (dataIdIt != this->dataIds_.end())
				{
					for (auto r : curTrialRecords)
					{
						if (r.dataId == *dataIdIt)
						{
							if (trialRecords.find(r.dataId) == trialRecords.end())
							{
								trialRecords[r.dataId] = std::vector<std::vector<record>>();
							}

							while (trialRecords[r.dataId].size() <= r.methodId)
							{
								trialRecords[r.dataId].push_back(std::vector<record>());
							}

							trialRecords[r.dataId][r.methodId].push_back(r);
						}
					}

					++dataIdIt;
				}

				for (auto dataRecords : trialRecords)
				{
					size_t methodId = 0;
					for (auto methodRecords : dataRecords.second)
					{
						std::map<size_t, record> line;
						for (auto r : methodRecords)
						{
							line[r.opId] = r;
						}

						if (line.size())
						{
							this->printLine(dataRecords.first * 10 + methodId, line);
						}

						++methodId;
					}
				}
				++curTrial;
			}
		}
		
		BOOST_LOG_TRIVIAL(info) << "#";		// print new line

		++expIdIt;
	}
}
void experimentRecorder::printLine(size_t tag, std::map<size_t, record>& line)
{
	if (line.size())
	{
		// Print if line exist
		std::stringstream ss;
		ss << "{" << tag << "} / ";	// dataId
		for (auto r : line)
		{
			ss << boost::format("%1$.5f") % r.second.procTime << ", ";		// procTimes
		}

		ss << " / ";
		for (auto r : line)
		{
			ss << "[" << r.second.opName << "] ";	// operator Names
		}

		BOOST_LOG_TRIVIAL(info) << ss.str();		// print
	}
}
}		// core
}		// msdb