#include <pch_test.h>
#include "zfp_lib.h"

namespace msdb
{
namespace test
{
int zfp_lib_compress_samplecode(double*& input, double*& output,
                                const size_t& inputSize, size_t& outputSize, 
                                int nx, int ny, int nz, 
                                double tolerance, int decompress)
{
    int status = 0;    /* return value: 0 = success */
    zfp_type type;     /* array scalar type */
    zfp_field* field;  /* array meta data */
    zfp_stream* zfp;   /* compressed stream */
    void* buffer;      /* storage for compressed stream */
    size_t bufsize;    /* byte size of compressed buffer */
    bitstream* stream; /* bit stream to write to or read from */
    size_t zfpsize;    /* byte size of compressed stream */

    /* allocate meta data for the 3D array a[nz][ny][nx] */
    type = zfp_type_double;
    field = zfp_field_3d(input, type, nx, ny, nz);

    /* allocate meta data for a compressed stream */
    zfp = zfp_stream_open(NULL);

    /* set compression mode and parameters via one of three functions */
    /*  zfp_stream_set_rate(zfp, rate, type, 3, 0); */
    /*  zfp_stream_set_precision(zfp, precision); */
    zfp_stream_set_accuracy(zfp, tolerance);

    /* compress or decompress entire array */
    if (decompress)
    {
        /* associate bit stream with allocated buffer */
        stream = stream_open(input, inputSize);
        zfp_stream_set_bit_stream(zfp, stream);
        zfp_stream_rewind(zfp);

        /* read compressed stream and decompress array */
        //zfpsize = fread(source, 1, inputSize, stdin);
        auto decompSize = zfp_decompress(zfp, field);
        if (!decompSize)
        {
            fprintf(stderr, "decompression failed\n");
            status = 1;
        }
        else
        {
            output = new double[decompSize];
            memcpy((void*)output, (void*)buffer, decompSize);
            outputSize = decompSize;
        }
    }
    else
    {
        /* allocate buffer for compressed data */
        bufsize = zfp_stream_maximum_size(zfp, field);
        buffer = (void*)(new char[bufsize]);

        /* associate bit stream with allocated buffer */
        stream = stream_open(buffer, bufsize);
        zfp_stream_set_bit_stream(zfp, stream);
        zfp_stream_rewind(zfp);

        /* compress array and output compressed stream */
        zfpsize = zfp_compress(zfp, field);
        if (!zfpsize)
        {
            fprintf(stderr, "compression failed\n");
            status = 1;
        }
        else
        {
            output = new double[zfpsize];
            memcpy((void*)output, (void*)buffer, zfpsize);
            //fwrite(buffer, 1, zfpsize, stdout);

            outputSize = zfpsize;
        }
    }

    /* clean up */
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);

    delete[] buffer;

    return status;
}

int zfp_lib_3d_compress(double*& input, double*& output, 
                        const size_t& inputSize, size_t& outputSize, 
                        int nx, int ny, int nz)
{
    int status = 0;                     /* return value: 0 = success */
    zfp_type type = zfp_type_double;     /* array scalar type */

    zfp_field* inputField = zfp_field_alloc();
    zfp_field_set_type(inputField, type);
    zfp_field_set_pointer(inputField, input);
    zfp_field_set_size_3d(inputField, nx, ny, nz);
    size_t n = zfp_field_size(inputField, NULL);

    zfp_stream* stream = zfp_stream_open(0);
    zfp_stream_set_reversible(stream);
    size_t bufsize = zfp_stream_maximum_size(stream, inputField);
    uchar* buffer = new uchar[bufsize];
    bitstream* bs = stream_open(buffer, bufsize);
    zfp_stream_set_bit_stream(stream, bs);
    zfp_stream_rewind(stream);

    outputSize = zfp_compress(stream, inputField);
    double ratio = double(n * sizeof(double)) / outputSize;

    output = new double[outputSize / sizeof(double)];

    memcpy((void*)output, buffer, outputSize);

    zfp_stream_close(stream);
    zfp_field_free(inputField);

    return status;
}
int zfp_lib_3d_decompress(double*& source, double*& output, 
                          const size_t& inputSize, size_t& outputSize, 
                          int nx, int ny, int nz)
{
    int status = 0;                     /* return value: 0 = success */
    zfp_type type = zfp_type_double;     /* array scalar type */

    output = new double[nx * ny * nz];

    zfp_field* outputField = zfp_field_alloc();
    zfp_field_set_type(outputField, type);
    zfp_field_set_pointer(outputField, output);
    zfp_field_set_size_3d(outputField, nx, ny, nz);
    size_t n = zfp_field_size(outputField, NULL);

    zfp_stream* stream = zfp_stream_open(0);
    zfp_stream_set_reversible(stream);
    bitstream* bs = stream_open(source, inputSize);
    zfp_stream_set_bit_stream(stream, bs);
    zfp_stream_rewind(stream);

    outputSize = zfp_decompress(stream, outputField);

    return status;
}
}		// test
}		// msdb