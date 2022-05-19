﻿#pragma once
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

        uchar* buffer = new uchar[bufsize * 1.5];                       // change bufsize -> bufsize * 1.5
        memset((void*)buffer, NULL, sizeof(uchar) * bufsize * 1.5);     // change bufsize -> bufsize * 1.5

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

        //auto curPos = os.capacity();
        //os.resize(curPos + outputSize);
        //memcpy((char*)(os.data()) + curPos - 1, buffer, outputSize);

        //output = new double[outputSize / sizeof(double)];
        //memcpy((void*)output, buffer, outputSize);

        zfp_stream_close(stream);
        zfp_field_free(inputField);
        try
        {
            delete[] buffer;
        }
        catch (...)
        {
            BOOST_LOG_TRIVIAL(error) << "zfp delete[] buffer error";
        }
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
        output = new uint8_t[outputFieldSize * sizeof(int32_t) * 1.5];
    }
    else
    {
        outputFieldSize = this->getBuffer()->size() / sizeof(Ty_);
        output = new uint8_t[outputFieldSize * sizeof(Ty_) * 1.5];
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

    try
    {
        delete[] output;
    }
    catch (...)
    {
        BOOST_LOG_TRIVIAL(error) << "zfp delete[] buffer error";
    }

    if (status)
    {
        _MSDB_THROW(
            _MSDB_EXCEPTIONS_MSG(
                MSDB_EC_IO_ERROR, MSDB_ER_DESERIALIZE, 
                "Fail to deserialize zfpBlock (status: " + std::to_string(status) + ")"));
    }
}
template<typename Ty_>
inline void zfpBlock<Ty_>::serialize(std::stringstream& os)
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

        uchar* buffer = new uchar[bufsize * 1.5];                       // change bufsize -> bufsize * 1.5
        memset((void*)buffer, NULL, sizeof(uchar) * bufsize * 1.5);     // change bufsize -> bufsize * 1.5
        bitstream* bs = stream_open(buffer, bufsize);
        zfp_stream_set_bit_stream(stream, bs);
        zfp_stream_rewind(stream);

        int outputSize = zfp_compress(stream, inputField);
        double ratio = double(n * sizeof(double)) / outputSize;

        // TODO
        //os << outputSize;
        //os.write((char*)buffer, outputSize);

        os << outputSize;
        for (int i = 0; i < outputSize; ++i)
        {
            os << (unsigned char)buffer[i];
        }

        //auto curPos = os.capacity();
        //os.resize(curPos + outputSize);
        //memcpy((char*)(os.data()) + curPos - 1, buffer, outputSize);

        zfp_stream_close(stream);
        zfp_field_free(inputField);
        try
        {
            delete[] buffer;
        }
        catch (...)
        {
            std::cout << "buffer delete error";
        }
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
//////////////////////////////
// zfpChunk
//
template<typename Ty_>
inline void zfpChunk<Ty_>::serialize(std::ostream& os)
{
    bstream bs;
    auto it = this->getBlockIterator();
    while (!it->isEnd())
    {
        (*it)->serialize(bs);
        ++(*it);
    }
    this->serializedSize_ = bs.capacity();

    this->getOutHeader()->serialize(os);
    os.write(bs.data(), bs.capacity());

    //std::stringstream bs;
    //auto it = this->getBlockIterator();
    //while (!it->isEnd())
    //{
    //    std::shared_ptr<zfpBlock<Ty_>> pBlock = std::static_pointer_cast<zfpBlock<Ty_>>(**it);
    //    pBlock->serialize(bs);
    //    ++(*it);
    //}
    //bs.seekp(0, bs.end);
    //auto size = bs.tellg();
    //this->serializedSize_ = size;

    //this->getOutHeader()->serialize(os);
    //os.write(bs.str().c_str(), size);
}
template<typename Ty_>
inline void zfpChunk<Ty_>::deserialize(std::istream& is)
{
    this->getHeader()->deserialize(is);
    this->updateFromHeader();

    bstream bs;
    bs.resize(this->serializedSize_);
    is.read(bs.data(), this->serializedSize_);

    auto it = this->getBlockIterator();
    while (!it->isEnd())
    {
        (*it)->deserialize(bs);
        ++(*it);
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
}		// msdb
}		// core
