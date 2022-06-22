#include <pch.h>
#include <api_cpp/cpp_domain.h>

namespace msdb
{
//Coordinates::Coordinates(std::vector<int64_t> vecInt)
//	: coor_(vecInt)
//{
//
//}

Coordinates::Coordinates(core::coordinates coor)
	: coor_(coor)
{

}

Domain::Domain(Coordinates start, Coordinates end)
	: range_(start.getCoor(), end.getCoor())
{

}
std::string Domain::toString()
{
	return this->range_.toString();
}
}		// msdb