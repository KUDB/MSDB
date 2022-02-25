#ifndef _MSDB_CPP_DOMAIN_H_
#define _MSDB_CPP_DOMAIN_H_

#include <pch.h>
#include <util/coordinate.h>

namespace msdb
{
class Coordinates
{
public:
	using Dty_ = int64_t;

public:
	//Coordinates(std::vector vecInt);
	Coordinates(core::coordinates coor);

public:
	inline core::coordinates getCoor() const
	{
		return this->coor_;
	}

private:
	core::coordinates coor_;
};

class Domain
{
public:
	using Dty_ = int64_t;

public:
	Domain(Coordinates start, Coordinates end);

public:
	inline core::coordinatesRange getRange() const
	{
		return this->range_;
	}

	inline core::coordinates getStart() const
	{
		return this->range_.getSp();
	}

	inline core::coordinates getEnd() const
	{
		return this->range_.getEp();
	}

	std::string toString();

private:
	core::coordinatesRange range_;
};
}		// msdb
#endif	// _MSDB_CPP_DOMAIN_H_