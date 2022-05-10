#include <pch.h>
#include <util/dataType.h>

namespace msdb
{
namespace core
{
dataType string2dataType(std::string str)
{
	if (str == concreteTy<char>::name())
	{
		return concreteTy<char>();
	} else if (str == concreteTy<int8_t>::name())
	{
		return concreteTy<int8_t>();
	}
	else if (str == concreteTy<uint8_t>::name())
	{
		return concreteTy<uint8_t>();
	}
	else if (str == concreteTy<int16_t>::name())
	{
		return concreteTy<int16_t>();
	}
	else if (str == concreteTy<uint16_t>::name())
	{
		return concreteTy<uint16_t>();
	}
	else if (str == concreteTy<int32_t>::name())
	{
		return concreteTy<int32_t>();
	}
	else if (str == concreteTy<uint32_t>::name())
	{
		return concreteTy<uint32_t>();
	}
	else if (str == concreteTy<int64_t>::name())
	{
		return concreteTy<int64_t>();
	}
	else if (str == concreteTy<uint64_t>::name())
	{
		return concreteTy<uint64_t>();
	}
	else if (str == concreteTy<float>::name())
	{
		return concreteTy<float>();
	}
	else if (str == concreteTy<double>::name())
	{
		return concreteTy<double>();
	}
	else if (str == concreteTy<bool>::name())
	{
		return concreteTy<bool>();
	}
}
}		// core
}		// msdb