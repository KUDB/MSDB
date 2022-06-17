#include <pch.h>
#include <util/logger.h>

namespace msdb
{
namespace core
{
bool initBoostLogger()
{
	logging::add_file_log(
		keywords::file_name = "../logs/log_%Y%m%d_%H%M%S_%5N.log",	// file name pattern
		keywords::rotation_size = 10 * 1024 * 1024,					// rotate files every 10 MiB
		keywords::format = "[%TimeStamp%] %Message%",				// log record format
		keywords::auto_flush = true
	);
	
	boost::shared_ptr< logging::core > core = logging::core::get();
#ifdef NDEBUG
	core->set_filter(
		logging::trivial::severity >= logging::trivial::info
	);
#else
	core->set_filter(
		logging::trivial::severity >= logging::trivial::debug
	);
#endif
	
	core->add_global_attribute("LineID", attrs::counter< unsigned int >(1));
	core->add_global_attribute("TimeStamp", attrs::local_clock());

	return true;
}

namespace logger
{

}		// logger
}		// core
}		// msdb