#include "dummy.h"

namespace msdb
{
namespace dummy
{
void tearDownQuery(
	core::pTimer myTimer, size_t expId, size_t expTrial,
	size_t dataId, size_t methodId)
{
	myTimer->printTime();
	myTimer->recordTime(expId, expTrial, dataId, methodId);
}
}		// dummy
}		// msdb
