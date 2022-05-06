#pragma once
#ifndef _TTHRESH_MATHUTIL_H_
#define _TTHRESH_MATHUTIL_H_

#include <iostream>
#include <math.h>

namespace tthresh
{
#define M_PI       3.14159265358979323846   // pi

inline int64_t min(int64_t a, int64_t b)
{
    return (a < b) ? a : b;
}
inline int64_t max(int64_t a, int64_t b)
{
    return (a > b) ? a : b;
}
}		// tthresh
#endif	// _TTHRESH_MATHUTIL_H_