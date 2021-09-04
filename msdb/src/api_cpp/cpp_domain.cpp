#include <pch.h>
#include <api_cpp/cpp_domain.h>

namespace msdb
{
Coordinate::Coordinate(std::vector<int64_t> vecInt)
	: coor_(vecInt)
{

}
Domain::Domain(Coordinate start, Coordinate end)
	: range_(start.getCoor(), end.getCoor())
{

}
}		// msdb