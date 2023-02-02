#include <pch_op.h>
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

Domain::Domain(std::vector<Coordinates::Dty_> start, std::vector<Coordinates::Dty_> end)
	: range_(core::coordinates(start), core::coordinates(end))
{

}

std::string Domain::toString()
{
	return this->range_.toString();
}
}		// msdb