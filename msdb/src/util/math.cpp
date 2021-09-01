#include <pch.h>
#include <util/math.h>

namespace msdb
{
namespace core
{
int abs_(int num) noexcept
{
	return std::abs((int)num);
}
long int abs_(long int num) noexcept
{
	return std::abs((long int)num);
}
long long int abs_(long long int num) noexcept
{
	return std::abs((long long int)num);
}
float abs_(float num) noexcept
{
	return std::abs((float)num);
}
double abs_(double num) noexcept
{
	return std::abs((double)num);
}
unsigned int abs_(unsigned int num) noexcept
{
	return num;
}
unsigned long int abs_(unsigned long int num) noexcept
{
	return num;
}
unsigned long long int abs_(unsigned long long int num) noexcept
{
	return num;
}
int64_t intDivCeil(int64_t li, int64_t ri)
{
	return li / ri + (li % ri != 0);
}

unsigned long long calcMaxLimit(bit_cnt_type bits)
{
	if(bits)
		return  ~((unsigned long long)1 & 0) >> (sizeof(unsigned long long) * CHAR_BIT - bits);
	return 0;
}

unsigned long long calcMinLimit(bit_cnt_type bits)
{
	if(bits)
		return (unsigned long long)1 << (bits - 1);
	return 0;
}
}		// core
}		// msdb