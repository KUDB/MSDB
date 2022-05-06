#pragma once

#ifndef _TTHRESHLIB_H_
#define _TTHRESHLIB_H_

#include <iostream>
#include <vector>
#include <stack>
#include <chrono>
#include <string>
#include <fstream>
#include "tthreshMathUtil.h"
//#include "encode.hpp"
//#include "tthresh.hpp"
#include "tthreshTucker.h"
//#include "io.hpp"
#include <string.h>
#include <assert.h>
#include <Eigen/Dense>
#include <map>

typedef long double LDOUBLE;

using namespace std;
using namespace std::chrono;
using namespace Eigen;

#ifdef WIN32
#include <iso646.h> //for 'and' 'or' ...
#endif

// Avoids corruption of the input and output data on Windows/MS-DOS systems
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define _USE_MATH_DEFINES

namespace tthresh
{
// Size (in bytes) for all I/O buffers
#define CHUNK (1<<18)

// Rows whose squared norm is larger than this will be cropped away
#define AUTOCROP_THRESHOLD (1e-10)

// Compression parameters
enum Mode { none_mode, input_mode, compressed_mode, output_mode, io_type_mode, sizes_mode, target_mode, skip_bytes_mode };
enum Target { eps, rmse, psnr };

constexpr uint8_t CODE_VALUE_BITS = 32;
constexpr uint64_t MAX_CODE = (((uint64_t)1) << CODE_VALUE_BITS) - 1;
constexpr uint64_t ONE_FOURTH = (MAX_CODE + ((uint64_t)1)) / 4;
constexpr uint64_t ONE_HALF = ONE_FOURTH * 2;
constexpr uint64_t THREE_FOURTHS = ONE_FOURTH * 3;

struct zs {
    uint64_t rbytes, wbytes;
    int8_t rbit, wbit;
    //FILE* file; // File handle to read/write from/to
    ostream* _os;
    istream* _is;
    uint8_t inout[CHUNK]; // Buffer to write the results of inflation/deflation
    uint8_t buf[CHUNK]; // Buffer used for the read/write operations
    int32_t bufstart = 0;
    int32_t bufend = 0;
    size_t total_written_bytes = 0; // Used to compute the final file size
}; // Read/write state for zlib interfacing

void display_error(string msg);
bool is_number(string& s);

class tthreshCompressor
{
public:
    void cumulative_products(vector<uint32_t>& in, vector<size_t>& out);

    void start_timer(string message);
    void stop_timer();

    //////////////////////////////
    // io
    //////////////////////////////
    void open_write(string output_file);
    void write_stream(unsigned char* buf, size_t bytes_to_write);
    void open_wbit();

    // Assumption: to_write <= 64
    void write_bits(uint64_t bits, char to_write);
    void close_wbit();
    void close_write();

    /*********/
    // Reading
    /*********/
    // If read_bits() has been called, call close_rbit() before read_stream()
    void open_read(string input_file);
    void open_read(istream& input);
    void read_stream(uint8_t* buf, size_t bytes_to_read);
    void close_rbit();

    // Assumption: to_read <= BITS
    int64_t read_bits(char to_read);
    void close_read();
    //////////////////////////////

    //////////////////////////////
    // Encode
    //////////////////////////////
    inline void put_bit(char bit) {
        write_bits(bit, 1);
        encoding_bits++;
    }
    inline void put_bit_plus_pending(bool bit, int& pending_bits)
    {
        put_bit(bit);
        for (int i = 0; i < pending_bits; i++)
            put_bit(!bit);
        pending_bits = 0;
    }
    uint64_t encode(vector<uint64_t>& rle);

    //////////////////////////////
    // Decode
    //////////////////////////////
    void decode(vector<size_t>& rle);

    //////////////////////////////
    // Tucker
    //////////////////////////////
    // 
    // Reads a tensor in the buffer data of size s, and compresses it.
    // The factor matrices are output parameters
    void hosvd_compress(double* data, vector<MatrixXd>& Us, bool verbose);
    //
    // Reads a tensor in the buffer data of size s, and decompresses it in-place
    void hosvd_decompress(vector<double>& data, vector<MatrixXd>& Us, bool verbose, vector<Slice>& cutout);

    //////////////////////////////
    // Compress
    //////////////////////////////
    vector<uint64_t> encode_array(double* c, size_t size, double eps_target,
                                  bool is_core, bool verbose = false);

    // io_type: uchar, ushort, int, float, double
    double* compress(string input_file, string compressed_file, string io_type,
                     Target target, double target_value,
                     size_t skip_bytes,
                     bool verbose = false, bool debug = false);

    void compress(std::vector<uint32_t> dims,
                  char* input, size_t inputSize, std::ostream& output, string io_type,
                  Target target, double target_value, size_t skip_bytes,
                  bool verbose = false, bool debug = false);

    //////////////////////////////
    // Decompress
    //////////////////////////////
    vector<uint64_t> decode_array(size_t size, bool is_core, bool verbose, bool debug);
    vector<double> dequantize(vector<uint64_t>& current);

    void decompress(string compressed_file, string output_file, double* data, vector<Slice>& cutout, bool autocrop, bool verbose, bool debug);
    // data?? = nullptr
    void decompress(std::istream& input, std::ostream& output, double* data,
                    vector<Slice>& cutout, bool autocrop, bool verbose, bool debug);

public:
    // Tensor dimensionality, ranks and sizes. They are only set, never modified
    uint8_t n;
    vector<uint32_t> r;
    vector<size_t> rprod;
    vector<uint32_t> s, snew;
    vector<size_t> sprod, snewprod;

    zs _zs;

    stack<high_resolution_clock::time_point> times;

    uint64_t encoding_bits;

    //////////////////////////////
    // Compress
    int qneeded;

    double rle_time = 0;
    double raw_time = 0;

    double price = -1, total_bits_core = -1, eps_core = -1;
    size_t total_bits = 0;
    //////////////////////////////

    //////////////////////////////
    // Decompress
    double maximum;
    int q;
    size_t pointer;

    /////
    double decode_rle_time;
    double decode_raw_time;
    double unscramble_time;
    //////////////////////////////
};

}		// tthresh
#endif	// _TTHRESHLIB_H_