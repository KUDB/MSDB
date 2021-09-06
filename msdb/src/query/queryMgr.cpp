#include <pch.h>
#include <query\queryMgr.h>

namespace msdb
{
namespace core
{
queryMgr::queryMgr()
{
}
queryMgr::~queryMgr()
{
}
status queryMgr::querySubmit(pQuery qry)
{
	/**
	 * TODO:: Query submit in Query Manager
	 * 
	 * 01. Query planning / optimizing
	 * 02. Lock resources
	 * 03. Error handling
	 */
	return qry->process();
}
}		// core
}		// msdb
