#ifndef _MSDB_CPP_DOMAIN_H_
#define _MSDB_CPP_DOMAIN_H_

#include <pch_op.h>
#include <util/coordinate.h>

namespace msdb
{
class OP_API Coordinates
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

class OP_API Domain
{
public:
	using Dty_ = int64_t;

public:
	Domain(Coordinates start, Coordinates end);
	Domain(std::vector<Coordinates::Dty_> start, std::vector<Coordinates::Dty_> end);

public:
	inline core::range getRange() const
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
	core::range range_;
};
}		// msdb
#endif	// _MSDB_CPP_DOMAIN_H_