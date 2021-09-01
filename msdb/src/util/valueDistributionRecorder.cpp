#include <pch.h>
#include <util/valueDistributionRecorder.h>
#include <limits>

namespace msdb
{
namespace core
{
valueDistributionRecorder::valueDistributionRecorder()
{
	this->table_ = std::vector<std::vector<size_t>>(CHAR_BIT + 1);

	for (int i = 0; i <= CHAR_BIT; ++i)
	{
		this->table_[i] = std::vector<size_t>(pow(2, i), 0);
	}
}
valueDistributionRecorder::~valueDistributionRecorder()
{
}
void valueDistributionRecorder::increase(size_t bits, uint8_t value)
{
	assert(bits <= CHAR_BIT);

	this->table_[bits][value]++;
}

void valueDistributionRecorder::printTable()
{
	BOOST_LOG_TRIVIAL(info) << "Value Distirbution Recorder Table";

	for (int i = 0; i < this->table_.size(); ++i)
	{
		BOOST_LOG_TRIVIAL(info) << "==============================";
		BOOST_LOG_TRIVIAL(info) << "BITS: " << i;

		for (int j = 0; j < this->table_[i].size(); ++j)
		{
			BOOST_LOG_TRIVIAL(info) << j << ": " << this->table_[i][j];
		}
		BOOST_LOG_TRIVIAL(info) << "==============================";
	}
}
}		// core
}		// msdb