#include <pch_experiments.h>

#include "experiments_range.h"

namespace msdb
{
namespace experiments
{
Domain getRandomRange(const core::coordinates& dims, const float selectivity)
{
	int64_t area = dims.area();
	//float selectedArea = area * selectivity / 100.0;

	auto value = pow((double)(selectivity / 100.0), (double)(1.0 / dims.size()));
	//core::coordinates width = dims * pow((double)selectivity / 100.0, (double)1/dims.size());
	core::coordinates width = dims;
	for (int d = 0; d < dims.size(); ++d)
	{
		width[d] = value * width[d];
	}

	core::coordinates sp = dims;
	for (int d = 0; d < dims.size(); ++d)
	{
		sp[d] = rand() % (size_t)(sp[d] - width[d]);
	}

	core::coordinates ep = sp + width;

	return Domain(sp, ep);
}
}		// experiments
}		// msdb