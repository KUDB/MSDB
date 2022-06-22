/*
 * Copyright (c) 2016-2017, Rafael Ballester-Ripoll
 *                          (Visualization and MultiMedia Lab, University of Zurich),
 *                          rballester@ifi.uzh.ch
 *
 * Licensed under the LGPLv3.0 (https://github.com/rballester/tthresh/blob/master/LICENSE)
 */

#ifndef __TUCKER_HPP__
#define __TUCKER_HPP__

#include "tthreshSlice.h"
#include <Eigen/Dense>

#include <math.h>

using namespace std;
using namespace Eigen;

namespace tthresh
{
// Projects an unfolded core M into M_proj using the transformation matrix U.
// U is an output parameter and is computed as the HOSVD of the tensor (left singular
// vectors of M) and M is compressed using U.transpose().
void project(MatrixXd& M, MatrixXd& U, MatrixXd& M_proj);

// U is an input parameter and M is decompressed using U (sliced as appropriate)
void unproject(MatrixXd& M, MatrixXd& U, MatrixXd& M_proj, Slice slice);
}       // tthresh
#endif // TUCKER_HPP
