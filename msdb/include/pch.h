#pragma once

#ifndef _MSDB_STDAFX_H_
#define _MSDB_STDAFX_H_

#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <type_traits>
#include <algorithm>
#include <utility>
#include <sstream>

// For bitstream
#include <bitset>

// For file system
#include <iosfwd>
#include <fstream>
#include <filesystem>

#include <memory>

// For timer
#include <chrono>
#include <mutex>

////////////////////////////////////////
// BOOST LIBRARY
//
// For value
#include <boost/any.hpp>

// For timmer
#include <boost/format.hpp>

// For logger
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

// For thread pool
#include <boost/asio/io_service.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>

// For zip compression
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>

// For cassert
#define assertm(exp, msg) assert(((void)msg, exp))

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace attrs = boost::log::attributes;

#endif	// _MSDB_STDAFX_H_