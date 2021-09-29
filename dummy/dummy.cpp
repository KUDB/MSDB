#include "dummy.h"

namespace msdb
{
namespace dummy
{
void tearDownQuery(
	core::pQuery qry, size_t expId, size_t expTrial, 
	size_t dataId, size_t methodId)
{
	auto myTimer = qry->getTimer();
	myTimer->printTime();
	myTimer->recordTime(expId, expTrial, dataId, methodId);
}
}		// dummy
}		// msdb
