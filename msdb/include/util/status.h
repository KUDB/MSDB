#pragma once
#ifndef _MSDB_STATUS_H_
#define _MSDB_STATUS_H_

#include <pch.h>

namespace msdb
{
namespace core
{
using statusSubCodeType = uint32_t;

enum class statusSectionCode
{
	OK,
	ERR,
	INPROCESS
};
enum class statusOkCode : statusSubCodeType
{
	OK,
	COMPLETE
};
enum class statusErrCode : statusSubCodeType
{
	ERR
};

enum class statusInprocessCode : statusSubCodeType
{
	INPROCESS
};

class status
{
public:
	status(statusSectionCode code, statusSubCodeType subCode);

public:
	inline statusSectionCode getSectionCode()
	{
		return this->sectionCode_;
	}

	inline bool isOk()
	{
		return this->sectionCode_ == statusSectionCode::OK;
	}

	inline bool isError()
	{
		return this->sectionCode_ == statusSectionCode::ERR;
	}

	inline bool isInprocess()
	{
		return this->sectionCode_ == statusSectionCode::INPROCESS;
	}

private:
	statusSectionCode sectionCode_;
	statusSubCodeType subCode_;
};
}		// core
}		// msdb
#endif	// _MSDB_STATUS_H_