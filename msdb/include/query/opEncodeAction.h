#pragma once
#ifndef _MSDB_OPENCODEACTION_H_
#define _MSDB_OPENCODEACTION_H_

#include <pch.h>

namespace msdb
{
namespace core
{
class opEncodeAction;
using pEncodeAction = std::shared_ptr<opEncodeAction>;

class opEncodeAction : public std::enable_shared_from_this<opEncodeAction>
{
public:
	opEncodeAction();
	virtual ~opEncodeAction();

public:
};
}
}
#endif	// _MSDB_OPENCODEACTION_H_
