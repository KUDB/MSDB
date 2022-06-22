/*
 * Copyright (c) 2016-2017, Rafael Ballester-Ripoll
 *                          (Visualization and MultiMedia Lab, University of Zurich),
 *                          rballester@ifi.uzh.ch
 *
 * Licensed under the LGPLv3.0 (https://github.com/rballester/tthresh/blob/master/LICENSE)
 */

#ifndef __SLICE_HPP__
#define __SLICE_HPP__

 //#include "tthresh.hpp"
#include <string>
#include <sstream>
#include <cmath>
#include <assert.h>

using namespace std;

namespace tthresh
{
// Cutout/downsampling modes
enum Reduction { Downsampling, Box, Lanczos };

class Slice 
{
public:
    int32_t points[3] = { INT32_MAX, INT32_MAX, 1 };
    int32_t max_upper = INT32_MAX;
    Reduction reduction = Downsampling;

    Slice(int32_t lower, int32_t stride, int32_t upper);  // Create a slice from its data
    Slice(string description); // Create a slice from its NumPy-like description
    const int32_t get_size(); // Number of elements encompassed by the slice
    const bool is_standard(); // Whether it is the (0,1,-1) slice (equivalent to doing nothing)
    void update(uint32_t size);

    friend ostream& operator<<(ostream& os, const Slice& slice);
};

ostream& operator<<(ostream& os, const Slice& slice);
}       // tthresh
#endif // SLICE_HPP
