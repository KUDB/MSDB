#include <pch.h>
#include <io/io_util.h>

namespace msdb
{
namespace core
{
std::stringstream::pos_type size_of_stream(const std::stringstream& ss)
{
    std::streambuf* buf = ss.rdbuf();

    // Get the current position so we can restore it later
    std::stringstream::pos_type original = buf->pubseekoff(0, ss.cur, ss.out);

    // Seek to end and get the position
    std::stringstream::pos_type end = buf->pubseekoff(0, ss.end, ss.out);

    // Restore the position
    buf->pubseekpos(original, ss.out);

    return end;
}
}		// core
}		// msdb