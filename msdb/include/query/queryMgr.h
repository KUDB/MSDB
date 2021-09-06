#pragma once
#ifndef _MSDB_QUERYMGR_H_
#define _MSDB_QUERYMGR_H_

#include <pch.h>
#include <util/singleton.h>
#include <util/status.h>
#include <query/query.h>

namespace msdb
{
namespace core
{
class queryMgr : public singleton<queryMgr>
{
private:
	queryMgr();
	~queryMgr();

public:
	status querySubmit(pQuery qry);

	// TODO:: async query submit

protected:
	friend singleton<queryMgr>;
};
}		// core
}		// msdb
#endif	// _MSDB_QUERYMGR_H_