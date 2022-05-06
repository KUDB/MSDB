#include "tthreshTucker.h"
#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <map>
#include "tthreshMathUtil.h"

using namespace std;
using namespace Eigen;

namespace tthresh
{
void project(MatrixXd& M, MatrixXd& U, MatrixXd& M_proj)
{
    SelfAdjointEigenSolver < MatrixXd > es(M * M.transpose()); // M*M^T is symmetric -> faster eigenvalue computation
    VectorXd eigenvalues = es.eigenvalues().real();
    MatrixXd U_unsorted = es.eigenvectors().real();
    uint32_t s = M.rows();
    U = MatrixXd(s, s);
    // We sort the (eigenvalue, eigenvector) pairs in descending order
    vector < std::pair < double, uint32_t >>eigenvalues_sorted(s);
    for (uint32_t i = 0; i < s; ++i)
        eigenvalues_sorted[i] = pair < double, uint32_t >(-eigenvalues(i), i);
    sort(eigenvalues_sorted.begin(), eigenvalues_sorted.end());
    for (uint32_t i = 0; i < s; ++i)
        U.col(i) = U_unsorted.col(eigenvalues_sorted[i].second);
    M_proj = U.transpose() * M;
}
void unproject(MatrixXd& M, MatrixXd& U, MatrixXd& M_proj, Slice slice)
{
    if (not slice.is_standard())
    {
        if (slice.points[0] < 0 or slice.points[1] > U.rows())
        { // TODO put in decompress.hpp
            cout << "Error: the slicing falls out of the tensor size range" << endl;
            exit(1);
        }
        int8_t sign = (0 < slice.points[2]) - (slice.points[2] < 0);
        if ((sign < 0 and slice.points[0] < slice.points[1]) or (sign > 0 and slice.points[0] > slice.points[1]))
        {
            cout << "Error: unfeasible slicing" << endl;
            exit(1);
        }
        MatrixXd convolution = MatrixXd::Zero(slice.get_size(), U.rows()); // convolution*U convolves U along the columns
    #pragma omp parallel for
        for (int32_t i = 0; i < slice.get_size(); ++i)
        {
            switch (slice.reduction)
            {
            case Downsampling:
            {
                convolution(i, slice.points[0] + i * slice.points[2]) = 1; // Delta kernel
                break;
            }
            case Box:
            {
                int32_t start = slice.points[0] + i * slice.points[2] - slice.points[2] / 2;
                int32_t end = max(min(slice.points[0] + i * slice.points[2] + (slice.points[2] - slice.points[2] / 2), U.rows()), 0);
                double kernel_sum = 1. / abs(end - start);
                for (int32_t j = start; sign * j < sign * end; j += sign)
                    convolution(i, j) = kernel_sum; // Box kernel
                break;
            }
            case Lanczos:
            {
                double a = 2 * slice.points[2]; // Upscaled Lanczos window
                int32_t start = max(min(slice.points[0] + i * slice.points[2] - a, U.rows() - 1), 0); // Kernel support: [-a, a], clamped
                int32_t end = max(min(slice.points[0] + i * slice.points[2] + a + 1, U.rows()), -1);
                double center = slice.points[0] + i * slice.points[2];
                double kernel_sum = 0;
                for (int32_t j = start; sign * j < sign * end; j += sign)
                {
                    double x = (j - center) / abs(slice.points[2]); // Upscaled x
                    if (x == 0)
                        convolution(i, j) = 1;
                    else
                        convolution(i, j) = a * sin(M_PI * x) * sin(M_PI * x / a) / (M_PI * M_PI * x * x); // Lanczos 2 kernel
                    kernel_sum += convolution(i, j);
                }
                for (int32_t j = start; sign * j < sign * end; j += sign)
                    convolution(i, j) /= kernel_sum; // Normalize the kernel so that it adds up to 1
                break;
            }
            }
        }
        M_proj = (convolution * U) * M;
    }
    else
        M_proj = U * M;
}
}		// tthresh