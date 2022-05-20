#include "tthreshCompressor.h"

namespace tthresh
{
void tthreshCompressor::cumulative_products(vector<uint32_t>& in, vector<size_t>& out)
{
    uint8_t n = s.size();
    out = vector<size_t>(n + 1); // Cumulative size products. The i-th element contains s[0]*...*s[i-1]
    out[0] = 1;
    for (uint8_t i = 0; i < n; ++i)
        out[i + 1] = out[i] * in[i];
}
void tthreshCompressor::start_timer(string message)
{
    cout << message << flush;
    times.push(std::chrono::high_resolution_clock::now());
}
void tthreshCompressor::stop_timer()
{
    if (times.size() < 1)
    {
        cout << "Error: timer not set" << endl;
        exit(1);
    }
    auto elapsed = std::chrono::high_resolution_clock::now() - times.top();
    times.pop();
    cout << "Elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() / 1000. << "ms" << endl << flush;
}
void tthreshCompressor::open_write(string output_file)
{
    //SET_BINARY_MODE(output_file.c_str());
    //_zs.file = fopen(output_file.c_str(), "wb");
}
void tthreshCompressor::write_stream(unsigned char* buf, size_t bytes_to_write)
{
    //fwrite(buf, 1, bytes_to_write, _zs.file);
    _zs._os->write((char*)buf, bytes_to_write);
    _zs.total_written_bytes += bytes_to_write;
}
void tthreshCompressor::open_wbit()
{
    _zs.wbytes = 0;
    _zs.wbit = 63;
}
void tthreshCompressor::write_bits(uint64_t bits, char to_write)
{
    if (to_write <= _zs.wbit + 1)
    {
        _zs.wbytes |= bits << (_zs.wbit + 1 - to_write);
        _zs.wbit -= to_write;
    }
    else
    {
        if (_zs.wbit > -1)
            _zs.wbytes |= bits >> (to_write - (_zs.wbit + 1));
        write_stream(reinterpret_cast<unsigned char*> (&_zs.wbytes), sizeof(_zs.wbytes));
        to_write -= _zs.wbit + 1;
        _zs.wbytes = 0;
        _zs.wbytes |= bits << (64 - to_write);
        _zs.wbit = 63 - to_write;
    }
}
void tthreshCompressor::close_wbit()
{
    // Write any reamining bits
    if (_zs.wbit < 63)
        write_stream(reinterpret_cast <unsigned char*> (&_zs.wbytes), sizeof(_zs.wbytes));
}
void tthreshCompressor::close_write()
{
    //fclose(_zs.file);
}
void tthreshCompressor::open_read(string input_file)
{
    //SET_BINARY_MODE(input_file.c_str());
    //_zs.file = fopen(input_file.c_str(), "rb");
    _zs.rbytes = 0;
    _zs.rbit = -1;
}

void tthreshCompressor::open_read(istream& input)
{
    //SET_BINARY_MODE(input_file.c_str());
    //_zs.file = fopen(input_file.c_str(), "rb");
    _zs._is = &input;
    _zs.rbytes = 0;
    _zs.rbit = -1;
}

void tthreshCompressor::read_stream(uint8_t* buf, size_t bytes_to_read)
{
    //size_t howmany = fread(buf, 1, bytes_to_read, _zs.file);
    size_t howmany = _zs._is->readsome((char*)buf, bytes_to_read);
    if (howmany != bytes_to_read)
    {
        cout << "Error: tried to read " << bytes_to_read << " bytes, got only " << howmany << endl;
        exit(1);
    }
}

void tthreshCompressor::close_rbit()
{
    _zs.rbytes = 0;
    _zs.rbit = -1;
}

int64_t tthreshCompressor::read_bits(char to_read)
{
    uint64_t result = 0;
    if (to_read <= _zs.rbit + 1)
    {
        result = _zs.rbytes << (63 - _zs.rbit) >> (64 - to_read);
        _zs.rbit -= to_read;
    }
    else
    {
        if (_zs.rbit > -1)
            result = _zs.rbytes << (64 - _zs.rbit - 1) >> (64 - to_read);
        read_stream(reinterpret_cast<uint8_t*> (&_zs.rbytes), sizeof(_zs.rbytes));
        to_read -= _zs.rbit + 1;
        result |= _zs.rbytes >> (64 - to_read);
        _zs.rbit = 63 - to_read;
    }
    return result;
}

void tthreshCompressor::close_read()
{
    //fclose(_zs.file);
}

uint64_t tthreshCompressor::encode(vector<uint64_t>& rle)
{

    // Build table of frequencies/probability intervals
    // key -> (count, lower bound)
    std::map<uint64_t, pair<uint64_t, uint64_t> > frequencies;
    for (uint64_t i = 0; i < rle.size(); ++i)
        ++frequencies[rle[i]].first;
    uint64_t count = 0;
    for (std::map<uint64_t, pair<uint64_t, uint64_t> >::iterator it = frequencies.begin(); it != frequencies.end(); ++it)
    {
        (it->second).second = count;
        count += (it->second).first;
    }

    encoding_bits = 0;

    //    open_wbit();

        //*********
        //********* Write frequencies
        //*********

        // Number of key/frequency pairs
    uint64_t dict_size = frequencies.size();
    write_bits(dict_size, sizeof(uint64_t) * 8);
    //    cerr << "dict_size: " << dict_size << endl;
    encoding_bits += sizeof(uint64_t) * 8;

    // Key/code pairs
    for (std::map<uint64_t, pair<uint64_t, uint64_t> >::iterator it = frequencies.begin(); it != frequencies.end(); ++it)
    {

        uint64_t key = it->first;
        uint64_t freq = (it->second).first;

        // First, the key's length
        uint8_t key_len = 0;
        uint64_t key_copy = key;
        while (key_copy)
        {
            key_copy >>= 1;
            key_len++;
        }
        key_len = max(1, key_len); // A 0 still requires 1 bit for us
        write_bits(key_len, 6);

        // Next, the key itself
        write_bits(key, key_len);

        // Now, the frequency's length
        uint8_t freq_len = 0;
        uint64_t freq_copy = freq;
        while (freq_copy)
        {
            freq_copy >>= 1;
            freq_len++;
        }
        freq_len = max(1, freq_len); // A 0 still requires 1 bit for us
        write_bits(freq_len, 6);

        // Finally, the frequency itself
        write_bits(freq, freq_len);

        encoding_bits += 6 + key_len + 6 + freq_len;
    }

    // Number N of symbols to code
    uint64_t n_symbols = rle.size();
    write_bits(n_symbols, sizeof(uint64_t) * 8);
    encoding_bits += sizeof(uint64_t) * 8;

    //*********
    //********* Write the encoding
    //*********

    int pending_bits = 0;
    uint64_t low = 0;
    uint64_t high = MAX_CODE;

    uint64_t rle_pos = 0;
    for (; ; )
    {
        uint64_t c = rle[rle_pos];
        rle_pos++;

        uint64_t phigh = frequencies[c].second + frequencies[c].first;
        uint64_t plow = frequencies[c].second;

        uint64_t range = high - low + 1;
        high = low + (range * phigh / n_symbols) - 1;
        low = low + (range * plow / n_symbols);

        for (; ; )
        {
            if (high < ONE_HALF)
                put_bit_plus_pending(0, pending_bits);
            else if (low >= ONE_HALF)
                put_bit_plus_pending(1, pending_bits);
            else if (low >= ONE_FOURTH && high < THREE_FOURTHS)
            {
                pending_bits++;
                low -= ONE_FOURTH;
                high -= ONE_FOURTH;
            }
            else
                break;
            high <<= 1;
            high++;
            low <<= 1;
            high &= MAX_CODE;
            low &= MAX_CODE;
        }

        if (rle_pos == n_symbols)
            break;
    }
    pending_bits++;
    if (low < ONE_FOURTH)
        put_bit_plus_pending(0, pending_bits);
    else
        put_bit_plus_pending(1, pending_bits);

    write_bits(0UL, CODE_VALUE_BITS - 2); // Trailing zeros
    encoding_bits += CODE_VALUE_BITS - 2;

    //    close_wbit();

    return encoding_bits;
}
void tthreshCompressor::decode(vector<size_t>& rle)
{

    //    open_rbit();

        //*********
        //********* Read the frequencies
        //*********

        // Number of key/frequency pairs
    uint64_t dict_size = read_bits(sizeof(uint64_t) * 8);

    // Pairs of key -> probability's lower bound
    std::map<uint64_t, uint64_t> lowers;

    uint64_t count = 0;
    for (uint64_t i = 0; i < dict_size; ++i)
    {

        // First, the key's length
        uint8_t key_len = read_bits(6);

        // Next, the key itself
        uint64_t key = read_bits(key_len);

        // Now, the frequency's length
        uint8_t freq_len = read_bits(6);

        // Finally, the frequency itself
        uint64_t freq = read_bits(freq_len);
        lowers[count] = key;
        count += freq;
    }

    // Number of symbols to translate back
    uint64_t n_symbols = read_bits(sizeof(uint64_t) * 8);

    lowers[n_symbols] = 0; // The last upper bound

    //*********
    //********* Read the encoding
    //*********

    uint64_t high = MAX_CODE;
    uint64_t low = 0;
    uint64_t value = 0;
    value = read_bits(CODE_VALUE_BITS);

    for (; ; )
    {

        uint64_t range = high - low + 1;
        uint64_t scaled_value = ((value - low + 1) * n_symbols - 1) / range;

        std::map<uint64_t, uint64_t>::iterator it = lowers.upper_bound(scaled_value);
        uint64_t phigh = it->first;
        it--;
        uint64_t c = it->second;
        rle.push_back(c);
        uint64_t plow = it->first;

        high = low + (range * phigh) / n_symbols - 1;
        low = low + (range * plow) / n_symbols;

        for (; ; )
        {

            if (high < ONE_HALF)
            {
                //do nothing, bit is a zero
            }
            else if (low >= ONE_HALF)
            {
                value -= ONE_HALF;  //subtract one half from all three code values
                low -= ONE_HALF;
                high -= ONE_HALF;
            }
            else if (low >= ONE_FOURTH && high < THREE_FOURTHS)
            {
                value -= ONE_FOURTH;
                low -= ONE_FOURTH;
                high -= ONE_FOURTH;
            }
            else
                break;
            low <<= 1;
            high <<= 1;
            high++;
            value <<= 1;
            value += read_bits(1) ? 1 : 0;
        }

        if (rle.size() == n_symbols)
            break;
    }
}
void tthreshCompressor::hosvd_compress(double* data, vector<MatrixXd>& Us, bool verbose)
{
    char n = s.size();

    // First unfolding: special case (elements are already arranged as we want)
    if (verbose) cout << "\tUnfold (1)... " << flush;
    MatrixXd M = MatrixXd::Map(data, s[0], sprod[n] / s[0]);
    MatrixXd M_proj;
    if (verbose) cout << "Project (1)..." << flush;
    project(M, Us[0], M_proj);
    if (verbose) cout << endl;

    // Remaining unfoldings: all of them go matrix -> matrix
    // Input: matrix of size s[dim-1] x (s[0] * ... * s[dim-2] * s[dim] * ... * s[N])
    // Output: matrix of size s[dim] x (s[0] * ... * s[dim-1] * s[dim+1] * ... * s[N])
    for (uint8_t dim = 1; dim < n; ++dim)
    {
        if (verbose) cout << "\tUnfold (" << dim + 1 << ")... " << flush;
        M = MatrixXd(s[dim], sprod[n] / s[dim]); // dim-th factor matrix
    #pragma omp parallel for
        for (int64_t j = 0; j < M_proj.cols(); ++j)
        {
            uint32_t write_i = (j / sprod[dim - 1]) % s[dim];
            size_t base_write_j = j % sprod[dim - 1] + j / (sprod[dim - 1] * s[dim]) * sprod[dim];
            for (int32_t i = 0; i < M_proj.rows(); ++i)
                M(write_i, base_write_j + i * sprod[dim - 1]) = M_proj(i, j);
        }
        if (verbose) cout << "\tProject (" << dim + 1 << ")... " << flush;
        project(M, Us[dim], M_proj);
        if (verbose) cout << endl;
    }

    // We fold back from matrix into ND tensor
    if (verbose) cout << "\tFold... " << flush << endl;
#pragma omp parallel for
    for (int32_t i = 0; i < int32_t(s[n - 1]); i++)
        for (size_t j = 0; j < sprod[n - 1]; j++)
            data[i * sprod[n - 1] + j] = M_proj(i, j);
}
void tthreshCompressor::hosvd_decompress(vector<double>& data, vector<MatrixXd>& Us, bool verbose, vector<Slice>& cutout)
{
    if (rprod[n] == 0)
    { // Extreme case: 0 ranks
        data = vector<double>(snewprod[n], 0); // Produce a 0 reconstruction of the expected size, and leave
        return;
    }

    // First unfolding: special case (elements are already arranged as we want)
    if (verbose) cout << "\tUnfold (1)... " << flush;
    MatrixXd M = MatrixXd::Map(data.data(), r[0], rprod[n] / r[0]);
    MatrixXd M_proj;
    if (verbose)
    {
        cout << "\tUnproject (" << 1 << ")";
        if (not cutout[0].is_standard())
            cout << " with cutout " << cutout[0];
        cout << "... " << flush;
    }
    unproject(M, Us[0], M_proj, cutout[0]);
    if (verbose) cout << endl;

    // Remaining unfoldings: all of them go matrix -> matrix
    // Input: matrix of size s[dim-1] x (s[0] * ... * s[dim-2] * s[dim] * ... * s[N])
    // Output: matrix of size s[dim] x (s[0] * ... * s[dim-1] * s[dim+1] * ... * s[N])
    for (uint8_t dim = 1; dim < n; ++dim)
    {
        if (verbose) cout << "\tUnfold (" << dim + 1 << ")... " << flush;
        M = MatrixXd(r[dim], snewprod[dim] * rprod[n] / rprod[dim + 1]); // dim-th factor matrix
    #pragma omp parallel for
        for (int64_t j = 0; j < M_proj.cols(); ++j)
        {
            uint32_t write_i = (j / snewprod[dim - 1]) % r[dim];
            size_t base_write_j = j % snewprod[dim - 1] + j / (snewprod[dim - 1] * r[dim]) * snewprod[dim];
            for (int32_t i = 0; i < M_proj.rows(); ++i)
                M(write_i, base_write_j + i * snewprod[dim - 1]) = M_proj(i, j);
        }
        if (verbose)
        {
            cout << "\tUnproject (" << dim + 1 << ")";
            if (not cutout[dim].is_standard())
                cout << " with cutout " << cutout[dim];
            cout << "... " << flush;
        }
        unproject(M, Us[dim], M_proj, cutout[dim]);
        if (verbose) cout << endl;
    }

    // We fold back from matrix into ND tensor
    if (verbose) cout << "\tFold... " << flush << endl;
    data.resize(snewprod[n]);
    data.shrink_to_fit();
#pragma omp parallel for
    for (ptrdiff_t i = 0; i < ptrdiff_t(snewprod[n]); i++)
        data[i] = M_proj(i / snewprod[n - 1], i % snewprod[n - 1]);
}
vector<uint64_t> tthreshCompressor::encode_array(double* c, size_t size, double eps_target, bool is_core, bool verbose)
{

    /**********************************************/
    // Compute and save maximum (in absolute value)
    /**********************************************/

    if (is_core and verbose)
        start_timer("Preliminaries... ");
    double maximum = 0;
    for (size_t i = 0; i < size; i++)
    {
        if (abs(c[i]) > maximum)
            maximum = abs(c[i]);
    }
    double scale = ldexp(1, 63 - ilogb(maximum));

    uint64_t tmp;
    memcpy(&tmp, (void*)&scale, sizeof(scale));
    write_bits(tmp, 64);

    // Vector of quantized core coefficients
    vector<uint64_t> coreq(size);
    for (size_t pos = 0; pos < size; ++pos)
    {
        coreq[pos] = uint64_t(abs(c[pos]) * scale);
    }

    // Kahan summation to compute the sum of squared core coefficients
    LDOUBLE s0 = 0;
    LDOUBLE s1 = 0;
    for (size_t pos = 0; pos < size; ++pos)
    {
        LDOUBLE t0 = s0;
        s1 += LDOUBLE(c[pos]) * c[pos];
        s0 += s1;
        s1 += t0 - s0;
    }
    LDOUBLE normsq = s0 * scale * scale;

    LDOUBLE sse = normsq;
    LDOUBLE last_eps = 1;
    LDOUBLE thresh = eps_target * eps_target * normsq;

    /**************/
    // Encode array
    /**************/

    vector<uint64_t> current(size, 0);

    if (is_core and verbose)
        stop_timer();
    bool done = false;
    total_bits = 0;
    size_t last_total_bits = total_bits;
    double eps_delta = 0, size_delta = 0, epsilon;
    int q;
    bool all_raw = false;
    if (verbose)
        start_timer("Encoding core...\n");
    for (q = 63; q >= 0; --q)
    {
        if (verbose and is_core)
            cout << "Encoding core's bit plane p = " << q;
        vector<uint64_t> rle;
        LDOUBLE plane_sse = 0;
        size_t plane_ones = 0;
        size_t counter = 0;
        size_t i;
        vector<bool> raw;
        for (i = 0; i < size; ++i)
        {
            bool current_bit = ((coreq[i] >> q) & 1UL);
            plane_ones += current_bit;
            if (not all_raw and current[i] == 0)
            { // Feed to RLE
                if (not current_bit)
                    counter++;
                else
                {
                    rle.push_back(counter);
                    counter = 0;
                }
            }
            else
            { // Feed to raw stream
                ++total_bits;
                raw.push_back(current_bit);
            }

            if (current_bit)
            {
                plane_sse += (LDOUBLE(coreq[i] - current[i]));
                current[i] |= ((uint64_t)1) << q;
                if (plane_ones % 100 == 0)
                {
                    LDOUBLE k = ((uint64_t)1) << q;
                    LDOUBLE sse_now = sse + (-2 * k * plane_sse + k * k * plane_ones);
                    if (sse_now <= thresh)
                    {
                        done = true;
                        if (verbose)
                            cout << " <- breakpoint: coefficient " << i;
                        break;
                    }
                }

            }
        }
        if (verbose and is_core)
            cout << endl;

        LDOUBLE k = ((uint64_t)1) << q;
        sse += -2 * k * plane_sse + k * k * plane_ones;
        rle.push_back(counter);

        uint64_t rawsize = raw.size();
        write_bits(rawsize, 64);
        total_bits += 64;

        {
            high_resolution_clock::time_point timenow = chrono::high_resolution_clock::now();
            for (size_t i = 0; i < raw.size(); ++i)
                write_bits(raw[i], 1);
            raw_time += std::chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - timenow).count() / 1000.;
        }
        {
            high_resolution_clock::time_point timenow = chrono::high_resolution_clock::now();
            uint64_t this_part = encode(rle);
            rle_time += std::chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - timenow).count() / 1000.;
            total_bits += this_part;
        }

        epsilon = sqrt(double(sse / normsq));
        if (last_total_bits > 0)
        {
            if (is_core)
            {
                size_delta = (total_bits - last_total_bits) / double(last_total_bits);
                eps_delta = (last_eps - epsilon) / epsilon;
            }
            else
            {
                if ((total_bits / total_bits_core) / (epsilon / eps_core) >= price)
                    done = true;
            }
        }
        last_total_bits = total_bits;
        last_eps = epsilon;

        if (raw.size() / double(size) > 0.8)
            all_raw = true;

        write_bits(all_raw, 1);
        total_bits++;

        write_bits(done, 1);
        total_bits++;

        if (done)
            break;
    }
    if (verbose)
        stop_timer();

    /****************************************/
    // Save signs of significant coefficients
    /****************************************/

    for (size_t i = 0; i < size; ++i)
    {
        if (current[i] > 0)
        {
            write_bits((c[i] > 0), 1);
            total_bits++;
        }
    }

    if (is_core)
    {
        price = size_delta / eps_delta;
        eps_core = epsilon;
        total_bits_core = total_bits;
    }
    return current;
}
double* tthreshCompressor::compress(string input_file, string compressed_file,
                                    string io_type, Target target, double target_value,
                                    size_t skip_bytes, bool verbose, bool debug)
{
    n = s.size();
    if (verbose)
    {
        cout << endl << "/***** Compression: " << to_string(n) << "D tensor of size " << s[0];
        for (uint8_t i = 1; i < n; ++i)
            cout << " x " << s[i];
        cout << " *****/" << endl << endl;
    }
    cumulative_products(s, sprod);

    /***********************/
    // Check input data type
    /***********************/

    size_t size = sprod[n]; // Total number of tensor elements
    uint8_t io_type_size, io_type_code;
    if (io_type == "uchar")
    {
        io_type_size = sizeof(unsigned char);
        io_type_code = 0;
    }
    else if (io_type == "ushort")
    {
        io_type_size = sizeof(unsigned short);
        io_type_code = 1;
    }
    else if (io_type == "int")
    {
        io_type_size = sizeof(int);
        io_type_code = 2;
    }
    else if (io_type == "float")
    {
        io_type_size = sizeof(float);
        io_type_code = 3;
    }
    else
    {
        io_type_size = sizeof(double);
        io_type_code = 4;
    }

    /************************/
    // Check input file sizes
    /************************/

    size_t expected_size = skip_bytes + size * io_type_size;
    ifstream input_stream(input_file.c_str(), ios::in | ios::binary);
    if (!input_stream.is_open())
    {
        cout << "Error: could not open \"" << input_file << "\"" << endl;
        exit(1);
    }
    size_t fsize = input_stream.tellg(); // Check that buffer size matches expected size
    input_stream.seekg(0, ios::end);
    fsize = size_t(input_stream.tellg()) - fsize;
    if (expected_size != fsize)
    {
        cout << "Invalid file size: expected ";
        if (skip_bytes > 0)
            cout << skip_bytes << " + ";
        cout << "(" << s[0];
        for (uint8_t i = 1; i < n; ++i)
            cout << "*" << s[i];
        cout << ")*" << int(io_type_size) << " = " << expected_size << " bytes, but found " << fsize << " bytes";
        if (expected_size > fsize)
            cout << " (" << expected_size / double(fsize) << " times too small)";
        else
            cout << " (" << fsize / double(expected_size) << " times too large)";
        if (skip_bytes == 0 and expected_size < fsize and fsize < expected_size + 1000)
        {
            cout << ". Perhaps the file has a header (use flag -k)?";
        }
        cout << endl;
        exit(1);
    }

    /********************************************/
    // Save tensor dimensionality, sizes and type
    /********************************************/

    open_write(compressed_file.c_str());
    write_stream(reinterpret_cast <unsigned char*> (&n), sizeof(n));
    write_stream(reinterpret_cast <unsigned char*> (&s[0]), n * sizeof(s[0]));
    write_stream(reinterpret_cast <unsigned char*> (&io_type_code), sizeof(io_type_code));

    /*****************************/
    // Load input file into memory
    /*****************************/

    if (verbose)
        start_timer("Loading and casting input data... ");
    input_stream.seekg(0, ios::beg);
    char* in = new char[size * io_type_size];
    input_stream.read(in, size * io_type_size);
    input_stream.close();

    // Cast the data to doubles
    double* data;
    double datamin = numeric_limits < double >::max(); // Tensor statistics
    double datamax = numeric_limits < double >::min();
    double datanorm = 0;
    if (io_type == "double")
        data = (double*)in + skip_bytes;
    else
        data = new double[size];
    for (size_t i = 0; i < size; ++i)
    {
        switch (io_type_code)
        {
        case 0:
            data[i] = *reinterpret_cast<unsigned char*>(&in[skip_bytes + i * io_type_size]);
            break;
        case 1:
            data[i] = *reinterpret_cast<unsigned short*>(&in[skip_bytes + i * io_type_size]);
            break;
        case 2:
            data[i] = *reinterpret_cast<int*>(&in[skip_bytes + i * io_type_size]);
            break;
        case 3:
            data[i] = *reinterpret_cast<float*>(&in[skip_bytes + i * io_type_size]);
            break;
        }
        datamin = min(datamin, data[i]); // Compute statistics, since we're at it
        datamax = max(datamax, data[i]);
        datanorm += data[i] * data[i];
    }
    datanorm = sqrt(datanorm);
    if (io_type_code != 4)
        delete[]in;
    if (verbose)
        stop_timer();
    if (debug) cout << "Input statistics: min = " << datamin << ", max = " << datamax << ", norm = " << datanorm << endl;

    /**********************************************************************/
    // Compute the target SSE (sum of squared errors) from the given metric
    /**********************************************************************/

    double sse;
    if (target == eps)
        sse = pow(target_value * datanorm, 2);
    else if (target == rmse)
        sse = pow(target_value, 2) * size;
    else
        sse = pow((datamax - datamin) / (2 * (pow(10, target_value / 20))), 2) * size;
    double epsilon = sqrt(sse) / datanorm;
    if (verbose)
    {
        double rmse = sqrt(sse / size);
        double psnr = 20 * log10((datamax - datamin) / (2 * rmse));
        cout << "We target eps = " << epsilon << ", rmse = " << rmse << ", psnr = " << psnr << endl;
    }

    /*********************************/
    // Create and decompose the tensor
    /*********************************/

    if (verbose)
        start_timer("Tucker decomposition...\n");
    double* c = new double[size]; // Tucker core

    memcpy(c, data, size * sizeof(double));

    vector<MatrixXd> Us(n); // Tucker factor matrices
    hosvd_compress(c, Us, verbose);

    if (verbose)
    {
        stop_timer();
        //        cout << "RLE time (ms):" << rle_time << endl;
        //        cout << "Raw time (ms):" << raw_time << endl;
    }

    /**************************/
    // Encode and save the core
    /**************************/

    open_wbit();
    vector<uint64_t> current = encode_array(c, size, epsilon, true, verbose);
    close_wbit();

    /*******************************/
    // Compute and save tensor ranks
    /*******************************/

    if (verbose)
        start_timer("Computing ranks... ");
    r = vector<uint32_t>(n, 0);
    vector<size_t> indices(n, 0);
    vector< RowVectorXd > slicenorms(n);
    for (int dim = 0; dim < n; ++dim)
    {
        slicenorms[dim] = RowVectorXd(s[dim]);
        slicenorms[dim].setZero();
    }
    for (size_t i = 0; i < size; ++i)
    {
        if (current[i] > 0)
        {
            for (int dim = 0; dim < n; ++dim)
            {
                slicenorms[dim][indices[dim]] += double(current[i]) * current[i];
            }
        }
        indices[0]++;
        int pos = 0;
        while (indices[pos] >= s[pos] and pos < n - 1)
        {
            indices[pos] = 0;
            pos++;
            indices[pos]++;
        }
    }

    for (int dim = 0; dim < n; ++dim)
    {
        for (size_t i = 0; i < s[dim]; ++i)
        {
            if (slicenorms[dim][i] > 0)
                r[dim] = i + 1;
            slicenorms[dim][i] = sqrt(slicenorms[dim][i]);
        }
    }
    if (verbose)
        stop_timer();

    if (verbose)
    {
        cout << "Compressed tensor ranks:";
        for (uint8_t i = 0; i < n; ++i)
            cout << " " << r[i];
        cout << endl;
    }
    write_stream(reinterpret_cast<unsigned char*> (&r[0]), n * sizeof(r[0]));

    for (uint8_t i = 0; i < n; ++i)
    {
        write_stream(reinterpret_cast<uint8_t*> (slicenorms[i].data()), r[i] * sizeof(double));
    }

    vector<MatrixXd> Uweighteds;
    open_wbit();
    for (int dim = 0; dim < n; ++dim)
    {
        MatrixXd Uweighted = Us[dim].leftCols(r[dim]);
        for (size_t col = 0; col < r[dim]; ++col)
            Uweighted.col(col) = Uweighted.col(col) * slicenorms[dim][col];
        Uweighteds.push_back(Uweighted);
        encode_array(Uweighted.data(), s[dim] * r[dim], 0, false);//*(s[i]*s[i]/sprod[n]));  // TODO flatten in F order?
    }
    close_wbit();
    close_write();
    delete[] c;
    size_t newbits = _zs.total_written_bytes * 8;
    if (verbose)
    {
        cout << "oldbits = " << size * io_type_size * 8L << ", newbits = " << newbits << ", compressionratio = " << size * io_type_size * 8L / double(newbits)
            << ", bpv = " << newbits / double(size) << endl << flush;
    }

    return data;
}
void tthreshCompressor::compress(std::vector<uint32_t> dims,
                                 char* input, size_t inputSize, 
                                 std::ostream& output, string io_type, Target target, double target_value,
                                 size_t skip_bytes, bool verbose, bool debug)
{
    s = dims;
    _zs._os = &output;
    n = s.size();
    if (verbose)
    {
        cout << endl << "/***** Compression: " << to_string(n) << "D tensor of size " << s[0];
        for (uint8_t i = 1; i < n; ++i)
            cout << " x " << s[i];
        cout << " *****/" << endl << endl;
    }
    cumulative_products(s, sprod);

    /***********************/
    // Check input data type
    /***********************/

    size_t size = sprod[n]; // Total number of tensor elements
    uint8_t io_type_size, io_type_code;
    if (io_type == "uchar")
    {
        io_type_size = sizeof(unsigned char);
        io_type_code = 0;
    }
    else if (io_type == "ushort")
    {
        io_type_size = sizeof(unsigned short);
        io_type_code = 1;
    }
    else if (io_type == "int")
    {
        io_type_size = sizeof(int);
        io_type_code = 2;
    }
    else if (io_type == "float")
    {
        io_type_size = sizeof(float);
        io_type_code = 3;
    }
    else
    {
        io_type_size = sizeof(double);
        io_type_code = 4;
    }

    /************************/
    // Check input file sizes
    /************************/

    //size_t expected_size = skip_bytes + size * io_type_size;
    //ifstream input_stream(input_file.c_str(), ios::in | ios::binary);
    //if (!input_stream.is_open())
    //{
    //    cout << "Error: could not open \"" << input_file << "\"" << endl;
    //    exit(1);
    //}
    //size_t fsize = input_stream.tellg(); // Check that buffer size matches expected size
    //input_stream.seekg(0, ios::end);
    //fsize = size_t(input_stream.tellg()) - fsize;
    //if (expected_size != fsize)
    //{
    //    cout << "Invalid file size: expected ";
    //    if (skip_bytes > 0)
    //        cout << skip_bytes << " + ";
    //    cout << "(" << s[0];
    //    for (uint8_t i = 1; i < n; ++i)
    //        cout << "*" << s[i];
    //    cout << ")*" << int(io_type_size) << " = " << expected_size << " bytes, but found " << fsize << " bytes";
    //    if (expected_size > fsize)
    //        cout << " (" << expected_size / double(fsize) << " times too small)";
    //    else
    //        cout << " (" << fsize / double(expected_size) << " times too large)";
    //    if (skip_bytes == 0 and expected_size < fsize and fsize < expected_size + 1000)
    //    {
    //        cout << ". Perhaps the file has a header (use flag -k)?";
    //    }
    //    cout << endl;
    //    exit(1);
    //}

    /********************************************/
    // Save tensor dimensionality, sizes and type
    /********************************************/

    //open_write(compressed_file.c_str());
    write_stream(reinterpret_cast <unsigned char*> (&n), sizeof(n));                  // original size
    write_stream(reinterpret_cast <unsigned char*> (&s[0]), n * sizeof(s[0]));        // dimension desc
    write_stream(reinterpret_cast <unsigned char*> (&io_type_code), sizeof(io_type_code));    // type code

    /*****************************/
    // Load input file into memory
    /*****************************/

    //if (verbose)
    //    start_timer("Loading and casting input data... ");
    //input_stream.seekg(0, ios::beg);
    //char* in = new char[size * io_type_size];
    //input_stream.read(in, size * io_type_size);
    //input_stream.close();

    // Cast the data to doubles
    double* data;
    double datamin = numeric_limits < double >::max(); // Tensor statistics
    double datamax = numeric_limits < double >::min();
    double datanorm = 0;
    if (io_type == "double")
    {
        //data = (double*)in + skip_bytes;
        data = (double*)input + skip_bytes;
    }
    else
    {
        data = new double[size];
    }
    for (size_t i = 0; i < size; ++i)
    {
        switch (io_type_code)
        {
        case 0:
        {
            // uchar
            data[i] = *reinterpret_cast<unsigned char*>(&input[skip_bytes + i * io_type_size]);
            //data[i] = *reinterpret_cast<unsigned char*>(&in[skip_bytes + i * io_type_size]);
            break;
        }
        case 1:
        {
            //ushort
            data[i] = *reinterpret_cast<unsigned short*>(&input[skip_bytes + i * io_type_size]);
            //data[i] = *reinterpret_cast<unsigned short*>(&in[skip_bytes + i * io_type_size]);
            break;
        }
        case 2:
        {
            // int
            data[i] = *reinterpret_cast<int*>(&input[skip_bytes + i * io_type_size]);
            //data[i] = *reinterpret_cast<int*>(&in[skip_bytes + i * io_type_size]);
            break;
        }
        case 3:
        {
            // float
            data[i] = *reinterpret_cast<float*>(&input[skip_bytes + i * io_type_size]);
            //data[i] = *reinterpret_cast<float*>(&in[skip_bytes + i * io_type_size]);
            break;
        }
        }
        datamin = min(datamin, data[i]); // Compute statistics, since we're at it
        datamax = max(datamax, data[i]);
        datanorm += data[i] * data[i];
    }
    datanorm = sqrt(datanorm);
    //if (io_type_code != 4)
    //    delete[]in;
    if (verbose)
        stop_timer();
    if (debug) cout << "Input statistics: min = " << datamin << ", max = " << datamax << ", norm = " << datanorm << endl;

    /**********************************************************************/
    // Compute the target SSE (sum of squared errors) from the given metric
    /**********************************************************************/

    double sse;
    if (target == eps)
        sse = pow(target_value * datanorm, 2);
    else if (target == rmse)
        sse = pow(target_value, 2) * size;
    else
        sse = pow((datamax - datamin) / (2 * (pow(10, target_value / 20))), 2) * size;
    double epsilon = sqrt(sse) / datanorm;
    if (verbose)
    {
        double rmse = sqrt(sse / size);
        double psnr = 20 * log10((datamax - datamin) / (2 * rmse));
        cout << "We target eps = " << epsilon << ", rmse = " << rmse << ", psnr = " << psnr << endl;
    }


    /*********************************/
    // Create and decompose the tensor
    /*********************************/

    if (verbose)
        start_timer("Tucker decomposition...\n");

    double* c = new double[size]; // Tucker core
    memcpy(c, data, size * sizeof(double));

    vector<MatrixXd> Us(n); // Tucker factor matrices
    hosvd_compress(c, Us, verbose);

    if (verbose)
    {
        stop_timer();
        //        cout << "RLE time (ms):" << rle_time << endl;
        //        cout << "Raw time (ms):" << raw_time << endl;
    }

    /**************************/
    // Encode and save the core
    /**************************/

    open_wbit();
    vector<uint64_t> current = encode_array(c, size, epsilon, true, verbose);
    close_wbit();

    /*******************************/
    // Compute and save tensor ranks
    /*******************************/

    if (verbose)
        start_timer("Computing ranks... ");
    r = vector<uint32_t>(n, 0);
    vector<size_t> indices(n, 0);
    vector< RowVectorXd > slicenorms(n);
    for (int dim = 0; dim < n; ++dim)
    {
        slicenorms[dim] = RowVectorXd(s[dim]);
        slicenorms[dim].setZero();
    }
    for (size_t i = 0; i < size; ++i)
    {
        if (current[i] > 0)
        {
            for (int dim = 0; dim < n; ++dim)
            {
                slicenorms[dim][indices[dim]] += double(current[i]) * current[i];
            }
        }
        indices[0]++;
        int pos = 0;
        while (indices[pos] >= s[pos] and pos < n - 1)
        {
            indices[pos] = 0;
            pos++;
            indices[pos]++;
        }
    }

    for (int dim = 0; dim < n; ++dim)
    {
        for (size_t i = 0; i < s[dim]; ++i)
        {
            if (slicenorms[dim][i] > 0)
                r[dim] = i + 1;
            slicenorms[dim][i] = sqrt(slicenorms[dim][i]);
        }
    }
    if (verbose)
        stop_timer();

    if (verbose)
    {
        cout << "Compressed tensor ranks:";
        for (uint8_t i = 0; i < n; ++i)
            cout << " " << r[i];
        cout << endl;
    }
    write_stream(reinterpret_cast<unsigned char*> (&r[0]), n * sizeof(r[0]));

    for (uint8_t i = 0; i < n; ++i)
    {
        write_stream(reinterpret_cast<uint8_t*> (slicenorms[i].data()), r[i] * sizeof(double));
    }

    vector<MatrixXd> Uweighteds;
    open_wbit();
    for (int dim = 0; dim < n; ++dim)
    {
        MatrixXd Uweighted = Us[dim].leftCols(r[dim]);
        for (size_t col = 0; col < r[dim]; ++col)
            Uweighted.col(col) = Uweighted.col(col) * slicenorms[dim][col];
        Uweighteds.push_back(Uweighted);
        encode_array(Uweighted.data(), s[dim] * r[dim], 0, false);//*(s[i]*s[i]/sprod[n]));  // TODO flatten in F order?
    }
    close_wbit();
    close_write();
    delete[] c;
    delete[] data;
    size_t newbits = _zs.total_written_bytes * 8;
    if (verbose)
    {
        cout << "oldbits = " << size * io_type_size * 8L << ", newbits = " << newbits << ", compressionratio = " << size * io_type_size * 8L / double(newbits)
            << ", bpv = " << newbits / double(size) << endl << flush;
    }
}

//////////////////////////////
// Decompress
//////////////////////////////
vector<uint64_t> tthreshCompressor::decode_array(size_t size, bool is_core, bool verbose, bool debug)
{
    uint64_t tmp = read_bits(64);
    memcpy(&maximum, (void*)&tmp, sizeof(tmp));

    vector<uint64_t> current(size, 0);

    decode_rle_time = 0;
    decode_raw_time = 0;
    unscramble_time = 0;

    int zeros = 0;
    bool all_raw = false;
    if (verbose and is_core)
        start_timer("Decoding core...\n");
    for (q = 63; q >= 0; --q)
    {
        if (verbose and is_core)
            cout << "Decoding core's bit plane p = " << q << endl;
        uint64_t rawsize = read_bits(64);

        size_t read_from_rle = 0;
        size_t read_from_raw = 0;

        if (all_raw)
        {
            high_resolution_clock::time_point timenow = chrono::high_resolution_clock::now();
            for (uint64_t pointer = 0; pointer < rawsize; ++pointer)
            {
                current[pointer] |= read_bits(1) << q;
            }
            unscramble_time += std::chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - timenow).count() / 1000.;
            vector<size_t> rle;
            decode(rle);
        }
        else
        {
            vector<bool> raw;
            high_resolution_clock::time_point timenow = chrono::high_resolution_clock::now();
            for (uint64_t i = 0; i < rawsize; ++i)
                raw.push_back(read_bits(1));
            decode_raw_time += std::chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - timenow).count() / 1000.;

            vector<size_t> rle;
            timenow = chrono::high_resolution_clock::now();
            decode(rle);
            decode_rle_time += std::chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - timenow).count() / 1000.;

            int64_t raw_index = 0;
            int64_t rle_value = -1;
            int64_t rle_index = -1;

            timenow = chrono::high_resolution_clock::now();
            for (pointer = 0; pointer < size; ++pointer)
            {
                uint64_t this_bit = 0;
                if (not all_raw and current[pointer] == 0)
                { // Consume bit from RLE
                    if (rle_value == -1)
                    {
                        rle_index++;
                        if (rle_index == int64_t(rle.size()))
                            break;
                        rle_value = rle[rle_index];
                    }
                    if (rle_value >= 1)
                    {
                        read_from_rle++;
                        this_bit = 0;
                        rle_value--;
                    }
                    else if (rle_value == 0)
                    {
                        read_from_rle++;
                        this_bit = 1;
                        rle_index++;
                        if (rle_index == int64_t(rle.size()))
                            break;
                        rle_value = rle[rle_index];
                    }
                }
                else
                { // Consume bit from raw
                    if (raw_index == int64_t(raw.size()))
                        break;
                    this_bit = raw[raw_index];
                    read_from_raw++;
                    raw_index++;
                }
                if (this_bit)
                    current[pointer] |= this_bit << q;
            }
            unscramble_time += std::chrono::duration_cast<std::chrono::microseconds>(chrono::high_resolution_clock::now() - timenow).count() / 1000.;
        }

        all_raw = read_bits(1);

        bool done = read_bits(1);
        if (done)
            break;
        else
            zeros++;
    }
    if (debug)
        cout << "decode_rle_time=" << decode_rle_time << ", decode_raw_time=" << decode_raw_time << ", unscramble_time=" << unscramble_time << endl;
    if (verbose and is_core)
        stop_timer();
    return current;
}
vector<double> tthreshCompressor::dequantize(vector<uint64_t>& current)
{ // TODO after resize
    size_t size = current.size();
    vector<double> c(size, 0);
    for (size_t i = 0; i < size; ++i)
    {
        if (current[i] > 0)
        {
            if (i < pointer)
            {
                if (q >= 1)
                    current[i] += 1UL << (q - 1);
            }
            else
                current[i] += 1UL << q;
            char sign = read_bits(1);
            c[i] = double(current[i]) / maximum * (sign * 2 - 1);
        }
    }
    return c;
}
void tthreshCompressor::decompress(std::istream& input, std::ostream& output, double* data,
                                   vector<Slice>& cutout, bool autocrop, bool verbose, bool debug)
{

    /***************************************************/
    // Read output tensor dimensionality, sizes and type
    /***************************************************/

    //open_read(compressed_file);
    open_read(input);
    read_stream(reinterpret_cast<uint8_t*> (&n), sizeof(n));
    s = vector<uint32_t>(n);
    read_stream(reinterpret_cast<uint8_t*> (&s[0]), n * sizeof(s[0]));

    bool whole_reconstruction = cutout.size() == 0;
    if (cutout.size() < n) // Non-specified slicings are assumed to be the standard (0,1,-1)
        for (uint32_t j = cutout.size(); j < s.size(); ++j)
            cutout.push_back(Slice(0, -1, 1));

    cumulative_products(s, sprod);
    size_t size = sprod[n];
    snew = vector<uint32_t>(n);
    for (uint8_t i = 0; i < n; ++i)
    {
        cutout[i].update(s[i]);
        snew[i] = cutout[i].get_size();
    }
    cumulative_products(snew, snewprod);

    if (verbose)
    {
        cout << endl << "/***** Decompression: " << to_string(n) << "D tensor of size ";
        if (not whole_reconstruction)
        {
            cout << snew[0];
            for (uint8_t i = 1; i < n; ++i)
                cout << " x " << snew[i];
            cout << " (originally ";
        }
        cout << s[0];
        for (uint8_t i = 1; i < n; ++i)
            cout << " x " << s[i];
        if (not whole_reconstruction)
            cout << ")";

        cout << " *****/" << endl << endl;
    }

    uint8_t io_type_code;
    read_stream(reinterpret_cast<uint8_t*> (&io_type_code), sizeof(io_type_code));
    uint8_t io_type_size;
    if (io_type_code == 0)
        io_type_size = sizeof(unsigned char);
    else if (io_type_code == 1)
        io_type_size = sizeof(unsigned short);
    else if (io_type_code == 2)
        io_type_size = sizeof(int);
    else if (io_type_code == 3)
        io_type_size = sizeof(float);
    else
        io_type_size = sizeof(double);

    /*************/
    // Decode core
    /*************/

    vector<uint64_t> current = decode_array(sprod[n], true, verbose, debug);
    vector<double> c = dequantize(current);
    close_rbit();

    /*******************/
    // Read tensor ranks
    /*******************/

    r = vector<uint32_t>(n);
    read_stream(reinterpret_cast<uint8_t*> (&r[0]), n * sizeof(r[0]));
    rprod = vector<size_t>(n + 1);
    rprod[0] = 1;
    for (uint8_t i = 0; i < n; ++i)
        rprod[i + 1] = rprod[i] * r[i];
    if (verbose)
    {
        cout << "Compressed tensor ranks:";
        for (uint8_t i = 0; i < n; ++i)
            cout << " " << r[i];
        cout << endl;
    }

    vector<RowVectorXd> slicenorms(n);
    for (uint8_t i = 0; i < n; ++i)
    {
        slicenorms[i] = RowVectorXd(r[i]);
        for (uint64_t col = 0; col < r[i]; ++col)
        { // TODO faster
            double norm;
            read_stream(reinterpret_cast<uint8_t*> (&norm), sizeof(double));
            slicenorms[i][col] = norm;
        }
    }

    //**********************/
    // Reshape core in place
    //**********************/

    size_t index = 0; // Where to read from in the original core
    vector<size_t> indices(n, 0);
    uint8_t pos = 0;
    for (size_t i = 0; i < rprod[n]; ++i)
    { // i marks where to write in the new rank-reduced core
        c[i] = c[index];
        indices[0]++;
        index++;
        pos = 0;
        // We update all necessary indices in cascade, left to right. pos == n-1 => i == rprod[n]-1 => we are done
        while (indices[pos] >= r[pos] and pos < n - 1)
        {
            indices[pos] = 0;
            index += sprod[pos + 1] - r[pos] * sprod[pos];
            pos++;
            indices[pos]++;
        }
    }

    //*****************/
    // Reweight factors
    //*****************/

    vector< MatrixXd > Us;
    for (uint8_t i = 0; i < n; ++i)
    {
        vector<uint64_t> factorq = decode_array(s[i] * r[i], false, verbose, debug);
        vector<double> factor = dequantize(factorq);
        MatrixXd Uweighted(s[i], r[i]);
        memcpy(Uweighted.data(), (void*)factor.data(), sizeof(double) * s[i] * r[i]);
        MatrixXd U(s[i], r[i]);
        for (size_t col = 0; col < r[i]; ++col)
        {
            if (slicenorms[i][col] > 1e-10)
                U.col(col) = Uweighted.col(col) / slicenorms[i][col];
            else
                U.col(col) *= 0;
        }
        Us.push_back(U);
    }
    close_rbit();

    /*************************/
    // Autocrop (if requested)
    /*************************/

    if (autocrop)
    {
        cout << "autocrop =";
        for (uint8_t dim = 0; dim < n; ++dim)
        {
            uint32_t start_row = 0, end_row = 0;
            bool start_set = false;
            for (int i = 0; i < Us[dim].rows(); ++i)
            {
                double sqnorm = 0;
                for (int j = 0; j < Us[dim].cols(); ++j)
                    sqnorm += Us[dim](i, j) * Us[dim](i, j);
                if (sqnorm > AUTOCROP_THRESHOLD)
                {
                    if (not start_set)
                    {
                        start_row = i;
                        start_set = true;
                    }
                    end_row = i + 1;
                }
            }
            cutout[dim].points[0] = start_row;
            cutout[dim].points[1] = end_row;
            snew[dim] = end_row - start_row;
            cout << " " << start_row << ":" << end_row;
        }
        cout << endl;
        cumulative_products(snew, snewprod);
    }

    /************************/
    // Reconstruct the tensor
    /************************/

    if (verbose)
        start_timer("Reconstructing tensor...\n");
    hosvd_decompress(c, Us, verbose, cutout);
    if (verbose)
        stop_timer();

    /***********************************/
    // Cast and write the result on disk
    /***********************************/

    if (verbose)
        start_timer("Casting and saving final result... ");
    //ofstream output_stream(output_file.c_str(), ios::out | ios::binary);
    size_t buf_elems = CHUNK;
    vector<uint8_t> buffer(io_type_size * buf_elems);
    size_t buffer_wpos = 0;
    double sse = 0;
    double datanorm = 0;
    double datamin = std::numeric_limits < double >::max();
    double datamax = std::numeric_limits < double >::min();
    double remapped = 0;
    for (size_t i = 0; i < snewprod[n]; ++i)
    {
        if (io_type_code == 0)
        {
            remapped = (unsigned char)(round(max(0.0, min(double(std::numeric_limits<unsigned char>::max()), c[i]))));
            reinterpret_cast <unsigned char*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else if (io_type_code == 1)
        {
            remapped = (unsigned short)(round(max(0.0, min(double(std::numeric_limits<unsigned short>::max()), c[i]))));
            reinterpret_cast <unsigned short*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else if (io_type_code == 2)
        {
            remapped = int(round(max(std::numeric_limits<int>::min(), min(double(std::numeric_limits<int>::max()), c[i]))));;
            reinterpret_cast <int*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else if (io_type_code == 3)
        {
            remapped = float(c[i]);
            reinterpret_cast <float*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else
        {
            remapped = c[i];
            reinterpret_cast <double*>(&buffer[0])[buffer_wpos] = remapped;
        }
        buffer_wpos++;
        if (buffer_wpos == buf_elems)
        {
            buffer_wpos = 0;
            output.write(reinterpret_cast<const char*>(&buffer[0]), io_type_size* buf_elems);
            //output_stream.write(reinterpret_cast<const char*>(&buffer[0]), io_type_size * buf_elems);
        }
        if (whole_reconstruction and not autocrop and data != NULL)
        { // If needed, we compute the error statistics
            datanorm += data[i] * data[i];
            sse += (data[i] - remapped) * (data[i] - remapped);
            datamin = min(datamin, data[i]);
            datamax = max(datamax, data[i]);
        }
    }
    if (buffer_wpos > 0)
    {
        output.write(reinterpret_cast<const char*>(&buffer[0]), io_type_size * buffer_wpos);
        //output_stream.write(reinterpret_cast<const char*>(&buffer[0]), io_type_size * buffer_wpos);
    }
    //output_stream.close();
    if (verbose)
        stop_timer();

    if (whole_reconstruction and not autocrop and data != NULL)
    {
        datanorm = sqrt(datanorm);
        double eps = sqrt(sse) / datanorm;
        double rmse = sqrt(sse / size);
        double psnr = 20 * log10((datamax - datamin) / (2 * rmse));
        cout << "eps = " << eps << ", rmse = " << rmse << ", psnr = " << psnr << endl;
    }
}

void tthreshCompressor::decompress(string compressed_file, string output_file, double* data, vector<Slice>& cutout, bool autocrop, bool verbose, bool debug)
{

    /***************************************************/
    // Read output tensor dimensionality, sizes and type
    /***************************************************/

    open_read(compressed_file);
    read_stream(reinterpret_cast<uint8_t*> (&n), sizeof(n));
    s = vector<uint32_t>(n);
    read_stream(reinterpret_cast<uint8_t*> (&s[0]), n * sizeof(s[0]));

    bool whole_reconstruction = cutout.size() == 0;
    if (cutout.size() < n) // Non-specified slicings are assumed to be the standard (0,1,-1)
        for (uint32_t j = cutout.size(); j < s.size(); ++j)
            cutout.push_back(Slice(0, -1, 1));

    cumulative_products(s, sprod);
    size_t size = sprod[n];
    snew = vector<uint32_t>(n);
    for (uint8_t i = 0; i < n; ++i)
    {
        cutout[i].update(s[i]);
        snew[i] = cutout[i].get_size();
    }
    cumulative_products(snew, snewprod);

    if (verbose)
    {
        cout << endl << "/***** Decompression: " << to_string(n) << "D tensor of size ";
        if (not whole_reconstruction)
        {
            cout << snew[0];
            for (uint8_t i = 1; i < n; ++i)
                cout << " x " << snew[i];
            cout << " (originally ";
        }
        cout << s[0];
        for (uint8_t i = 1; i < n; ++i)
            cout << " x " << s[i];
        if (not whole_reconstruction)
            cout << ")";

        cout << " *****/" << endl << endl;
    }

    uint8_t io_type_code;
    read_stream(reinterpret_cast<uint8_t*> (&io_type_code), sizeof(io_type_code));
    uint8_t io_type_size;
    if (io_type_code == 0)
        io_type_size = sizeof(unsigned char);
    else if (io_type_code == 1)
        io_type_size = sizeof(unsigned short);
    else if (io_type_code == 2)
        io_type_size = sizeof(int);
    else if (io_type_code == 3)
        io_type_size = sizeof(float);
    else
        io_type_size = sizeof(double);

    /*************/
    // Decode core
    /*************/

    vector<uint64_t> current = decode_array(sprod[n], true, verbose, debug);
    vector<double> c = dequantize(current);
    close_rbit();

    /*******************/
    // Read tensor ranks
    /*******************/

    r = vector<uint32_t>(n);
    read_stream(reinterpret_cast<uint8_t*> (&r[0]), n * sizeof(r[0]));
    rprod = vector<size_t>(n + 1);
    rprod[0] = 1;
    for (uint8_t i = 0; i < n; ++i)
        rprod[i + 1] = rprod[i] * r[i];
    if (verbose)
    {
        cout << "Compressed tensor ranks:";
        for (uint8_t i = 0; i < n; ++i)
            cout << " " << r[i];
        cout << endl;
    }

    vector<RowVectorXd> slicenorms(n);
    for (uint8_t i = 0; i < n; ++i)
    {
        slicenorms[i] = RowVectorXd(r[i]);
        for (uint64_t col = 0; col < r[i]; ++col)
        { // TODO faster
            double norm;
            read_stream(reinterpret_cast<uint8_t*> (&norm), sizeof(double));
            slicenorms[i][col] = norm;
        }
    }

    //**********************/
    // Reshape core in place
    //**********************/

    size_t index = 0; // Where to read from in the original core
    vector<size_t> indices(n, 0);
    uint8_t pos = 0;
    for (size_t i = 0; i < rprod[n]; ++i)
    { // i marks where to write in the new rank-reduced core
        c[i] = c[index];
        indices[0]++;
        index++;
        pos = 0;
        // We update all necessary indices in cascade, left to right. pos == n-1 => i == rprod[n]-1 => we are done
        while (indices[pos] >= r[pos] and pos < n - 1)
        {
            indices[pos] = 0;
            index += sprod[pos + 1] - r[pos] * sprod[pos];
            pos++;
            indices[pos]++;
        }
    }

    //*****************/
    // Reweight factors
    //*****************/

    vector< MatrixXd > Us;
    for (uint8_t i = 0; i < n; ++i)
    {
        vector<uint64_t> factorq = decode_array(s[i] * r[i], false, verbose, debug);
        vector<double> factor = dequantize(factorq);
        MatrixXd Uweighted(s[i], r[i]);
        memcpy(Uweighted.data(), (void*)factor.data(), sizeof(double) * s[i] * r[i]);
        MatrixXd U(s[i], r[i]);
        for (size_t col = 0; col < r[i]; ++col)
        {
            if (slicenorms[i][col] > 1e-10)
                U.col(col) = Uweighted.col(col) / slicenorms[i][col];
            else
                U.col(col) *= 0;
        }
        Us.push_back(U);
    }
    close_rbit();

    /*************************/
    // Autocrop (if requested)
    /*************************/

    if (autocrop)
    {
        cout << "autocrop =";
        for (uint8_t dim = 0; dim < n; ++dim)
        {
            uint32_t start_row = 0, end_row = 0;
            bool start_set = false;
            for (int i = 0; i < Us[dim].rows(); ++i)
            {
                double sqnorm = 0;
                for (int j = 0; j < Us[dim].cols(); ++j)
                    sqnorm += Us[dim](i, j) * Us[dim](i, j);
                if (sqnorm > AUTOCROP_THRESHOLD)
                {
                    if (not start_set)
                    {
                        start_row = i;
                        start_set = true;
                    }
                    end_row = i + 1;
                }
            }
            cutout[dim].points[0] = start_row;
            cutout[dim].points[1] = end_row;
            snew[dim] = end_row - start_row;
            cout << " " << start_row << ":" << end_row;
        }
        cout << endl;
        cumulative_products(snew, snewprod);
    }

    /************************/
    // Reconstruct the tensor
    /************************/

    if (verbose)
        start_timer("Reconstructing tensor...\n");
    hosvd_decompress(c, Us, verbose, cutout);
    if (verbose)
        stop_timer();

    /***********************************/
    // Cast and write the result on disk
    /***********************************/

    if (verbose)
        start_timer("Casting and saving final result... ");
    ofstream output_stream(output_file.c_str(), ios::out | ios::binary);
    size_t buf_elems = CHUNK;
    vector<uint8_t> buffer(io_type_size * buf_elems);
    size_t buffer_wpos = 0;
    double sse = 0;
    double datanorm = 0;
    double datamin = std::numeric_limits < double >::max();
    double datamax = std::numeric_limits < double >::min();
    double remapped = 0;
    for (size_t i = 0; i < snewprod[n]; ++i)
    {
        if (io_type_code == 0)
        {
            remapped = (unsigned char)(round(max(0.0, min(double(std::numeric_limits<unsigned char>::max()), c[i]))));
            reinterpret_cast <unsigned char*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else if (io_type_code == 1)
        {
            remapped = (unsigned short)(round(max(0.0, min(double(std::numeric_limits<unsigned short>::max()), c[i]))));
            reinterpret_cast <unsigned short*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else if (io_type_code == 2)
        {
            remapped = int(round(max(std::numeric_limits<int>::min(), min(double(std::numeric_limits<int>::max()), c[i]))));;
            reinterpret_cast <int*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else if (io_type_code == 3)
        {
            remapped = float(c[i]);
            reinterpret_cast <float*>(&buffer[0])[buffer_wpos] = remapped;
        }
        else
        {
            remapped = c[i];
            reinterpret_cast <double*>(&buffer[0])[buffer_wpos] = remapped;
        }
        buffer_wpos++;
        if (buffer_wpos == buf_elems)
        {
            buffer_wpos = 0;
            output_stream.write(reinterpret_cast<const char*>(&buffer[0]), io_type_size * buf_elems);
        }
        if (whole_reconstruction and not autocrop and data != NULL)
        { // If needed, we compute the error statistics
            datanorm += data[i] * data[i];
            sse += (data[i] - remapped) * (data[i] - remapped);
            datamin = min(datamin, data[i]);
            datamax = max(datamax, data[i]);
        }
    }
    if (buffer_wpos > 0)
        output_stream.write(reinterpret_cast<const char*>(&buffer[0]), io_type_size * buffer_wpos);
    output_stream.close();
    if (verbose)
        stop_timer();

    if (whole_reconstruction and not autocrop and data != NULL)
    {
        datanorm = sqrt(datanorm);
        double eps = sqrt(sse) / datanorm;
        double rmse = sqrt(sse / size);
        double psnr = 20 * log10((datamax - datamin) / (2 * rmse));
        cout << "eps = " << eps << ", rmse = " << rmse << ", psnr = " << psnr << endl;
    }
}
void display_error(string msg)
{
    cout << endl;
    cout << "Error: " << msg << endl;
    cout << "Run \"tthresh -h\" for usage information" << endl;
    cout << endl;
    exit(1);
}
bool is_number(string& s)
{
    string::const_iterator it = s.begin();
    while (it != s.end() and (isdigit(*it) or *it == '.'))
        ++it;
    return !s.empty() and it == s.end();
}
}		// tthresh