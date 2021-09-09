#ifndef _MSDB_CPP_DOMAIN_H_
#define _MSDB_CPP_DOMAIN_H_

#include <pch.h>
#include <util/coordinate.h>

namespace msdb
{
class Coordinate
{
public:
	using Dty_ = int64_t;

public:
	Coordinate(std::vector<Dty_> vecInt);

public:
	inline core::coordinate<Dty_> getCoor() const
	{
		return this->coor_;
	}

private:
	core::coordinate<Dty_> coor_;
};

class Domain
{
public:
	using Dty_ = int64_t;

public:
	Domain(Coordinate start, Coordinate end);

public:
	inline core::coordinateRange<Dty_> getRange() const
	{
		return this->range_;
	}

	inline core::coordinate<Dty_> getStart() const
	{
		return this->range_.getSp();
	}

	inline core::coordinate<Dty_> getEnd() const
	{
		return this->range_.getEp();
	}

	std::string toString();

private:
	core::coordinateRange<Dty_> range_;
};
}		// msdb
#endif	// _MSDB_CPP_DOMAIN_H_