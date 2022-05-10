#pragma once
#include <op/zfp_encode/zfpChunk.h>
#include <zfp/zfp.h>

namespace msdb
{
namespace core
{
template<typename Ty_>
void zfpBlock<Ty_>::serialize(bstream& os)
{
    int status = 0;                         // return value: 0 = success
    zfp_type type = getZFPType<Ty_>();      // array scalar type
                                            // ZFP supports only int32, int64, float, double
    
    uint8_t* input = nullptr;
    bool useTempInput = false;

    try
    {
        input = (uint8_t*)this->getBuffer()->getData();
        size_t inputFieldSize = this->getDesc()->dims_.area();
        if (sizeof(Ty_) < sizeof(int32_t))
        {
            // To prevent out of range exception, use temp input buffer
            if (inputFieldSize % sizeof(int32_t))
            {
                inputFieldSize = std::ceil(((double)inputFieldSize) / sizeof(int32_t));
                input = new uint8_t[inputFieldSize * sizeof(int32_t)];
                useTempInput = true;
            }
            else
            {
                inputFieldSize /= sizeof(int32_t);
            }
        }

        zfp_field* inputField = zfp_field_alloc();
        zfp_field_set_type(inputField, type);
        zfp_field_set_pointer(inputField, input);
        // TODO:: check bufferSize of size / sizeof(Ty_)


        zfp_field_set_size_1d(inputField, inputFieldSize);
        size_t n = zfp_field_size(inputField, NULL);

        zfp_stream* stream = zfp_stream_open(0);
        zfp_stream_set_reversible(stream);
        size_t bufsize = zfp_stream_maximum_size(stream, inputField);
        uchar* buffer = new uchar[bufsize];
        bitstream* bs = stream_open(buffer, bufsize);
        zfp_stream_set_bit_stream(stream, bs);
        zfp_stream_rewind(stream);

        int64_t outputSize = zfp_compress(stream, inputField);
        double ratio = double(n * sizeof(double)) / outputSize;

        // TODO
        os << outputSize;
        for (int i = 0; i < outputSize; ++i)
        {
            os << buffer[i];
        }

        //output = new double[outputSize / sizeof(double)];
        //memcpy((void*)output, buffer, outputSize);

        zfp_stream_close(stream);
        zfp_field_free(inputField);
        // TODO:: delete[] buffer?
    }
    catch (...)
    {
        status = true;
    }

    if (useTempInput)
    {
        delete[] input;
    }

    if (status)
    {
        _MSDB_THROW(
            _MSDB_EXCEPTIONS_MSG(
                MSDB_EC_IO_ERROR, MSDB_ER_SERIALIZE, 
                "Fail to serialize zfpBlock (status: " + std::to_string(status) + ")"));
    }
}

template<typename Ty_>
void zfpBlock<Ty_>::deserialize(bstream& is)
{
    int status = 0;     // value: 0 = success
    zfp_type type = getZFPType<Ty_>();

    // TODO
    uint8_t* output = nullptr;
    size_t outputFieldSize = 0;

    if (sizeof(Ty_) < sizeof(int32_t))
    {
        outputFieldSize = std::ceil(((double)this->getBuffer()->size()) / sizeof(int32_t));
        output = new uint8_t[outputFieldSize * sizeof(int32_t)];
    }
    else
    {
        outputFieldSize = this->getBuffer()->size() / sizeof(Ty_);
        output = new uint8_t[outputFieldSize * sizeof(Ty_)];
    }
    
    zfp_field* outputField = zfp_field_alloc();
    zfp_field_set_type(outputField, type);
    zfp_field_set_pointer(outputField, output);
    // TODO:: check bufferSize of size / sizeof(Ty_)
    zfp_field_set_size_1d(outputField, outputFieldSize);        
    size_t n = zfp_field_size(outputField, NULL);

    int64_t inputSize = 0;
    is >> inputSize;
    if (inputSize == 0)
    {
        memset(this->getBuffer()->getData(), 0, this->getBuffer()->size());

        return;
    }

    //uint8_t* input = new uint8_t[inputSize];

    zfp_stream* stream = zfp_stream_open(0);
    zfp_stream_set_reversible(stream);
    uint8_t* pInputData = (uint8_t*)is.data() + sizeof(int64_t);
    bitstream* bs = stream_open(pInputData, inputSize);
    zfp_stream_set_bit_stream(stream, bs);
    zfp_stream_rewind(stream);

    auto outputSize = zfp_decompress(stream, outputField);

    memcpy(this->getBuffer()->getData(), output, this->getBuffer()->size());

    delete[] output;

    if (status)
    {
        _MSDB_THROW(
            _MSDB_EXCEPTIONS_MSG(
                MSDB_EC_IO_ERROR, MSDB_ER_DESERIALIZE, 
                "Fail to deserialize zfpBlock (status: " + std::to_string(status) + ")"));
    }
}

//////////////////////////////
// zfpChunkFactory
//
template <typename Ty_>
pChunk zfpChunkFactory<Ty_>::makeChunk(pChunkDesc cDesc)
{
	return std::make_shared<zfpChunk<Ty_>>(cDesc);
}

template<typename Ty_>
zfp_type getZFPType()
{
    if (typeid(Ty_) == typeid(bool) ||
        typeid(Ty_) == typeid(char) ||
        typeid(Ty_) == typeid(int8_t) ||
        typeid(Ty_) == typeid(uint8_t) ||
        typeid(Ty_) == typeid(int16_t) ||
        typeid(Ty_) == typeid(uint16_t) ||
        typeid(Ty_) == typeid(int32_t) ||
        typeid(Ty_) == typeid(uint32_t))
    {
        return zfp_type_int32;
    }

    if (typeid(Ty_) == typeid(int64_t) ||
        typeid(Ty_) == typeid(uint64_t))
    {
        return zfp_type_int64;
    }

    if (typeid(Ty_) == typeid(float))
    {
        return zfp_type_float;
    }

    if (typeid(Ty_) == typeid(double))
    {
        return zfp_type_double;
    }

    return zfp_type_none;
}
}		// msdb
}		// core
