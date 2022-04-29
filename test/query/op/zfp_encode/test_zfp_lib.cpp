#include <pch_test.h>
#include <dummy/dummy.h>
#include <op/zfp_encode/zfp_encode_action.h>
#include "zfp_lib.h"

namespace msdb
{
namespace test
{
TEST(zfp_lib, zfp_lib_encode)
{
    int nx = 100;
    int ny = 100;
    int nz = 100;
    double* array = new double[nx * ny * nz * sizeof(double)];
    double* output = nullptr;
    bool decompress = false;
    size_t sizeSrc = 0, sizeOutput = 0;

    /* initialize array to be compressed */
    int i, j, k;
    for (k = 0; k < nz; k++)
    {
        for (j = 0; j < ny; j++)
        {
            for (i = 0; i < nx; i++)
            {
                double x = 2.0 * i / nx;
                double y = 2.0 * j / ny;
                double z = 2.0 * k / nz;
                array[i + nx * (j + ny * k)] = exp(-(x * x + y * y + z * z));
            }
        }
    }

    zfp_lib_compress_samplecode(array, output, sizeSrc, sizeOutput, nx, ny, nz, 1e-3, decompress);

    delete[] array;
    if (output)
    {
        delete[] output;
    }
}

TEST(zfp_lib, zfp_lib_decode)
{
    int nx = 10;
    int ny = 10;
    int nz = 10;
    size_t srcSize = nx * ny * nz * sizeof(double);
    size_t compSize = 0;
    size_t decompSize = 0;
    double* srcArr = new double[srcSize];
    double* compArr = nullptr;
    double* decompArr = nullptr;
    bool isDecompress = false;


    /* initialize array to be compressed */
    int i, j, k;
    int no = 0;
    for (k = 0; k < nz; k++)
    {
        for (j = 0; j < ny; j++)
        {
            for (i = 0; i < nx; i++)
            {
                double x = 2.0 * i / nx;
                double y = 2.0 * j / ny;
                double z = 2.0 * k / nz;
                srcArr[i + nx * (j + ny * k)] = exp(-(x * x + y * y + z * z));
            }
        }
    }

    {
        zfp_lib_3d_compress(srcArr, compArr, srcSize, compSize, nx, ny, nz);
    }
    {
        isDecompress = true;
        zfp_lib_3d_decompress(compArr, decompArr, compSize, decompSize, nx, ny, nz);
    }

    //EXPECT_NE(srcSize, compSize);
    //EXPECT_EQ(srcSize, decompSize);
    EXPECT_FALSE(!memcmp(srcArr, compArr, compSize));
    EXPECT_TRUE(!memcmp(srcArr, decompArr, srcSize));

    delete[] srcArr;
    if (compArr)
    {
        delete[] compArr;
    }
    if (decompArr)
    {
        delete[] decompArr;
    }
}
}		// test
}		// msdb