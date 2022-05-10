#pragma once
#include <pch_test.h>
#include <zfp/zfp.h>

namespace msdb
{
namespace test
{
////////////////////////////////////////
// Sample code for ZFP encode/decode
//
int zfp_lib_compress_samplecode(double*& input, double*& output,
								const size_t& inputSize, size_t& outputSize,
								int nx, int ny, int nz, 
								double tolerance, int decompress);

int zfp_lib_3d_compress(double*& input, double*& output,
						const size_t& inputSize, size_t& outputSize, 
						int nx, int ny, int nz);

int zfp_lib_3d_decompress(double*& input, double*& output, 
						const size_t& inputSize, size_t& outputSize, 
						int nx, int ny, int nz);

template <typename Ty_>
int zfp_lib_compress(Ty_*& input, Ty_*& output,
					 const int64_t& inputBytes, int64_t& outputBytes)
{
    assert(inputBytes % sizeof(Ty_) == 0);
    assert(inputBytes % sizeof(int32_t) == 0);

    int status = 0;                             /* return value: 0 = success */
    zfp_type type = core::getZFPType<Ty_>();    /* array scalar type */

    zfp_field* inputField = zfp_field_alloc();
    zfp_field_set_type(inputField, type);
    zfp_field_set_pointer(inputField, input);
    if (sizeof(Ty_) < sizeof(int32_t))
    {
        zfp_field_set_size_1d(inputField, inputBytes / sizeof(int32_t));
    }
    else
    {
        zfp_field_set_size_1d(inputField, inputBytes / sizeof(Ty_));
    }
    size_t n = zfp_field_size(inputField, NULL);

    zfp_stream* stream = zfp_stream_open(0);
    zfp_stream_set_reversible(stream);
    size_t bufsize = zfp_stream_maximum_size(stream, inputField);
    uchar* buffer = new uchar[bufsize];
    bitstream* bs = stream_open(buffer, bufsize);
    zfp_stream_set_bit_stream(stream, bs);
    zfp_stream_rewind(stream);

    outputBytes = zfp_compress(stream, inputField);
    double ratio = double(n * sizeof(Ty_)) / outputBytes;

    output = new Ty_[outputBytes / sizeof(Ty_)];

    memcpy((void*)output, buffer, outputBytes);

    zfp_stream_close(stream);
    zfp_field_free(inputField);

    return status;
}
}		// test
}		// msdb