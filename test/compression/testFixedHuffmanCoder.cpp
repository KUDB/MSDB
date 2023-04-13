#include "gtest/gtest.h"

#include <compression/huffmanCode.h>
#include <compression/fixedHuffmanCoder.h>
#include <io/bitstream.h>
#include <util/math.h>

using namespace msdb::core;

std::vector<std::vector<iFixedHuffmanCoder*>> fixedHuffmanCoders =
{
	{
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr
	},
	{
		fixedHuffmanCoder<1, uint8_t>::instance(),
		fixedHuffmanCoder<1, uint8_t>::instance(),
		fixedHuffmanCoder<2, uint8_t>::instance(),
		fixedHuffmanCoder<3, uint8_t>::instance(),
		fixedHuffmanCoder<4, uint8_t>::instance(),
		fixedHuffmanCoder<5, uint8_t>::instance(),
		fixedHuffmanCoder<6, uint8_t>::instance(),
		fixedHuffmanCoder<7, uint8_t>::instance(),
		fixedHuffmanCoder<8, uint8_t>::instance(),
		fixedHuffmanCoder<9, uint8_t>::instance(),
		fixedHuffmanCoder<10, uint8_t>::instance(),
		fixedHuffmanCoder<11, uint8_t>::instance(),
		fixedHuffmanCoder<12, uint8_t>::instance(),
		fixedHuffmanCoder<13, uint8_t>::instance(),
		fixedHuffmanCoder<14, uint8_t>::instance(),
		fixedHuffmanCoder<15, uint8_t>::instance(),
		fixedHuffmanCoder<16, uint8_t>::instance()
	},
	{
		fixedHuffmanCoder<1, uint16_t>::instance(),
		fixedHuffmanCoder<1, uint16_t>::instance(),
		fixedHuffmanCoder<2, uint16_t>::instance(),
		fixedHuffmanCoder<3, uint16_t>::instance(),
		fixedHuffmanCoder<4, uint16_t>::instance(),
		fixedHuffmanCoder<5, uint16_t>::instance(),
		fixedHuffmanCoder<6, uint16_t>::instance(),
		fixedHuffmanCoder<7, uint16_t>::instance(),
		fixedHuffmanCoder<8, uint16_t>::instance(),
		fixedHuffmanCoder<9, uint16_t>::instance(),
		fixedHuffmanCoder<10, uint16_t>::instance(),
		fixedHuffmanCoder<11, uint16_t>::instance(),
		fixedHuffmanCoder<12, uint16_t>::instance(),
		fixedHuffmanCoder<13, uint16_t>::instance(),
		fixedHuffmanCoder<14, uint16_t>::instance(),
		fixedHuffmanCoder<15, uint16_t>::instance(),
		fixedHuffmanCoder<16, uint16_t>::instance()
	}
};

// Decode single item
template <typename Ty_>
Ty_ huffmanDecode(bstream& bs, iFixedHuffmanCoder* coder, const bit_cnt_type symbolBits)
{
	static size_t itemCapa = 1;

	// Read more buffer
	size_t tempBufferSize = std::min({ (size_t)(symbolBits * itemCapa * sizeof(Ty_) / (double)CHAR_BIT * 2) + 2, (size_t)(bs.capacity() - bs.getInBlockPos()) });
	bstream bsHuffman;
	bsHuffman.resize(tempBufferSize);
	memcpy(bsHuffman.data(), bs.data() + bs.getInBlockPos() * bs.getBlockBytes(), tempBufferSize);
	bsHuffman.jumpBits(bs.getInBitPosInBlock());

	// Decode item
	Ty_ huffmanDecoded = 0;
	auto readBits = coder->decode(static_cast<void*>(&huffmanDecoded), itemCapa, bsHuffman);
	BOOST_LOG_TRIVIAL(debug) << "ReadBits: " << readBits;
	bs.jumpBits(readBits);

	return huffmanDecoded;
}

// Decode various number of itmes in bulk
template <typename Ty_>
void huffmanDecode(Ty_* out, bstream& bs, iFixedHuffmanCoder* coder, const bit_cnt_type symbolBits, const size_t size)
{
	// Read more buffer
	size_t tempBufferSize = std::min({ (size_t)(symbolBits * size * sizeof(Ty_) / (double)CHAR_BIT * 2) + 2, (size_t)(bs.capacity() - bs.getInBlockPos()) });
	bstream bsHuffman;
	bsHuffman.resize(tempBufferSize);
	memcpy(bsHuffman.data(), bs.data() + bs.getInBlockPos() * bs.getBlockBytes(), tempBufferSize);
	bsHuffman.jumpBits(bs.getInBitPosInBlock());

	// Decode items
	auto readBits = coder->decode(static_cast<void*>(out), size, bsHuffman);
	BOOST_LOG_TRIVIAL(debug) << "ReadBits: " << readBits;
	bs.jumpBits(readBits);
}

template <typename Ty_, typename CoderTy_>
void huffmanEncode(bstream& bs, std::vector<Ty_>& vec, CoderTy_* coder)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		coder->encode(bs, &vec[i], CHAR_BIT);
	}
}

template <typename Ty_>
void getDummyData(std::vector<Ty_>& vec, const size_t size)
{
	for (int i = 0; i < size; ++i)
	{
		vec.push_back(i);
	}
}

TEST(fixed_huffman_coder_longer, HuffmanCoderLonger_init)
{
	auto coderLonger1_8 = fixedHuffmanCoderLonger<1, uint8_t>::instance();
	auto coderLonger2_8 = fixedHuffmanCoderLonger<2, uint8_t>::instance();
	auto coderLonger3_8 = fixedHuffmanCoderLonger<3, uint8_t>::instance();
	auto coderLonger4_16 = fixedHuffmanCoderLonger<4, uint16_t>::instance();
	auto coderLonger5_16 = fixedHuffmanCoderLonger<5, uint16_t>::instance();
	auto coderLonger6_16 = fixedHuffmanCoderLonger<6, uint16_t>::instance();
	auto coderLonger7_32 = fixedHuffmanCoderLonger<7, uint32_t>::instance();
	auto coderLonger8_32 = fixedHuffmanCoderLonger<8, uint32_t>::instance();

	EXPECT_TRUE(true);
}

TEST(fixed_huffman_coder_longer, HuffmanCoderLong_7_EncodeDecode)
{
	using data_type = char;
	const size_t symbolBits = 7;
	auto coder = fixedHuffmanCoderLonger<symbolBits, uint32_t>::instance();

	bstream bs;
	std::vector<data_type> vec;
	size_t size = pow((int)2, (int)symbolBits);
	getDummyData(vec, size);
	huffmanEncode(bs, vec, coder);

	////////////////////
	bs.print();		// Print Encoded Code Stream
	////////////////////

	BOOST_LOG_TRIVIAL(info) << "SymbolBits: " << symbolBits;
	for (int i = 0; i < size; ++i)
	{
		data_type v = huffmanDecode<data_type>(bs, coder, symbolBits);
		if (vec[i] != v)
		{
			std::cout << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
			BOOST_LOG_TRIVIAL(error) << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
		}
		else
		{
			BOOST_LOG_TRIVIAL(trace) << "Pass: " << static_cast<int64_t>(vec[i]);
		}
		EXPECT_EQ(vec[i], v);
	}
}

TEST(fixed_huffman_coder_longer, HuffmanCoderLong_7_EncodeDecodeBulk)
{
	using data_type = char;
	const size_t symbolBits = 7;
	auto coder = fixedHuffmanCoderLonger<symbolBits, uint32_t>::instance();

	bstream bs;
	std::vector<data_type> vec;
	size_t size = pow((int)2, (int)symbolBits);
	getDummyData(vec, size);
	huffmanEncode(bs, vec, coder);

	////////////////////
	bs.print();		// Print Encoded Code Stream
	////////////////////

	BOOST_LOG_TRIVIAL(info) << "SymbolBits: " << symbolBits;
	data_type* out = new data_type[size];

	huffmanDecode<data_type>(out, bs, coder, symbolBits, size);

	for (int i = 0; i < size; ++i)
	{
		if (vec[i] != out[i])
		{
			std::cout << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(out[i]) << std::endl;
			BOOST_LOG_TRIVIAL(error) << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(out[i]) << std::endl;
		}
		else
		{
			BOOST_LOG_TRIVIAL(trace) << "Pass: " << static_cast<int64_t>(vec[i]);
		}
		EXPECT_EQ(vec[i], out[i]);
	}

	delete[] out;
}

TEST(fixed_huffman_coder_longer, HuffmanCoderLong_8_EncodeDecode)
{
	using data_type = char;
	const size_t symbolBits = 8;
	auto coder = fixedHuffmanCoderLonger<symbolBits, uint32_t>::instance();

	bstream bs;
	std::vector<data_type> vec;
	size_t size = pow((int)2, (int)symbolBits);
	getDummyData(vec, size);
	huffmanEncode(bs, vec, coder);

	////////////////////
	bs.print();		// Print Encoded Code Stream
	////////////////////

	BOOST_LOG_TRIVIAL(info) << "SymbolBits: " << symbolBits;
	for (int i = 0; i < size; ++i)
	{
		data_type v = huffmanDecode<data_type>(bs, coder, symbolBits);
		if (vec[i] != v)
		{
			std::cout << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
			BOOST_LOG_TRIVIAL(error) << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
		}
		else
		{
			BOOST_LOG_TRIVIAL(trace) << "Pass: " << static_cast<int64_t>(vec[i]);
		}
		EXPECT_EQ(vec[i], v);
	}
}

TEST(fixed_huffman_coder, encode_decode_test)
{
	//EXPECT_TRUE(true);
	//using data_type = char;
	//size_t rbFromDelta = 8;
	//auto coder = fixedHuffmanCoders[sizeof(data_type)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(data_type) * CHAR_BIT) })];

	//bstream bs;

	//std::vector<data_type> vec;
	//size_t size = pow((int)2, (int)rbFromDelta);
	//for (int i = 0; i < size; ++i)
	//{
	//	vec.push_back(i);
	//	data_type value = i;
	//	coder->encode(bs, &value, 1);
	//}

	//BOOST_LOG_TRIVIAL(info) << "rbFromDelta: " << rbFromDelta;
	//for (int i = 0; i < size; ++i)
	//{
	//	data_type v = huffmanDecode<data_type>(bs, coder, rbFromDelta);
	//	if (vec[i] != v)
	//	{
	//		std::cout << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
	//		BOOST_LOG_TRIVIAL(error) << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
	//	}
	//	EXPECT_EQ(vec[i], v);
	//}
}