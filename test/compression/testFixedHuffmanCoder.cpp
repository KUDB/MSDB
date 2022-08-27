#include <pch_test.h>

#include <compression/huffmanCode.h>
#include <compression/fixedHuffmanCoder.h>
#include <io/bitstream.h>

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

template <typename Ty_>
Ty_ getHuffmanDecode(bstream& bs, iFixedHuffmanCoder* coder, bit_cnt_type rbFromDelta)
{
	static size_t itemCapa = 1;

	// Read more buffer
	size_t tempBufferSize = std::min({ (size_t)(rbFromDelta * itemCapa * sizeof(Ty_) / (double)CHAR_BIT * 2) + 2, (size_t)(bs.capacity() - bs.getInBlockPos()) });
	bstream bsHuffman;
	bsHuffman.resize(tempBufferSize);
	memcpy(bsHuffman.data(), bs.data() + bs.getInBlockPos() * bs.getBlockBytes(), tempBufferSize);
	bsHuffman.jumpBits(bs.getInBitPosInBlock());

	//Ty_ huffmanDecoded = new Ty_[itemCapa];
	//auto readBits = coder->decode(static_cast<void*>(huffmanDecoded), itemCapa, bsHuffman);
	////BOOST_LOG_TRIVIAL(debug) << "Bits: " << readBits;
	//bs.jumpBits(readBits);

	Ty_ huffmanDecoded = 0;
	auto readBits = coder->decode(static_cast<void*>(&huffmanDecoded), itemCapa, bsHuffman);
	//BOOST_LOG_TRIVIAL(debug) << "Bits: " << readBits;
	bs.jumpBits(readBits);

	return huffmanDecoded;
}

TEST(fixed_huffman_coder, encode_decode_test)
{
	EXPECT_TRUE(true);
	using data_type = char;
	size_t rbFromDelta = 8;
	auto coder = fixedHuffmanCoders[sizeof(data_type)][std::min({ (bit_cnt_type)rbFromDelta, (bit_cnt_type)(sizeof(data_type) * CHAR_BIT) })];

	bstream bs;

	std::vector<data_type> vec;
	size_t size = pow((int)2, (int)rbFromDelta);
	for (int i = 0; i < size; ++i)
	{
		vec.push_back(i);
		data_type value = i;
		coder->encode(bs, &value, 1);
	}

	BOOST_LOG_TRIVIAL(info) << "rbFromDelta: " << rbFromDelta;
	for (int i = 0; i < size; ++i)
	{
		data_type v = getHuffmanDecode<data_type>(bs, coder, rbFromDelta);
		if (vec[i] != v)
		{
			std::cout << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
			BOOST_LOG_TRIVIAL(error) << "Wrong->" << static_cast<int64_t>(vec[i]) << " : " << static_cast<int64_t>(v) << std::endl;
		}
		EXPECT_EQ(vec[i], v);
	}
}