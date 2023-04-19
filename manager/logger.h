#pragma once

#ifndef _MSDB_MANAGER_LOGGER_H_
#define _MSDB_MANAGER_LOGGER_H_

#include "pch_manager.h"

namespace msdb
{
namespace core
{
//////////////////////////////
// Trivial logging with filters
//////////////////////////////
//
// - trace
// - debug
// - info
// - warning
// - error
// - fatal
//

//MANAGER_DLL_API bool initBoostLogger();
//
//namespace logger
//{
//class msdbLogger
//{
//public:
//	msdbLogger(std::string tag)
//		: _tag(tag)
//	{
//		using backend_type = log_sinks::text_file_backend;
//		using sink_type = log_sinks::synchronous_sink<backend_type>;
//
//		auto backend = boost::make_shared<backend_type>(
//			log_keywords::file_name = "../logs/log_%Y%m%d_%H%M%S_%5N.log",	// file name pattern
//			log_keywords::rotation_size = 10 * 1024 * 1024,					// rotate files every 10 MiB
//			log_keywords::format = "[%TimeStamp%] %Message%",				// log record format
//			log_keywords::auto_flush = true
//		);
//
//		auto sink = boost::make_shared<sink_type>(backend);
//		//sink->set_formatter();
//		#ifdef NDEBUG
//			sink->set_filter(
//				logging::trivial::severity >= logging::trivial::info
//			);
//		#else
//			sink->set_filter(
//				logging::trivial::severity >= logging::trivial::debug
//			);
//		#endif
//
//		logging::core::get()->add_sink(sink);
//	};
//
//private:
//	const std::string _tag;
//};
//}		// logger
}		// core
}		// msdb
#endif	// _MSDB_MANAGER_LOGGER_H_
