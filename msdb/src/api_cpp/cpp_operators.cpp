#include <pch.h>
#include <api_cpp/cpp_operators.h>
#include <api_cpp/cpp_query.h>

namespace msdb
{
/* ************************ */
/* Load						*/
/* ************************ */
Load::Load(Array arr)
{
}

/* ************************ */
/* Between					*/
/* ************************ */
Between::Between(Array arr, Domain d)
	: parentQuery_(Load(arr)), domain_(d)
{

}

Between::Between(AFLQuery q, Domain d)
	: parentQuery_(q), domain_(d)
{

}
}		// msdb