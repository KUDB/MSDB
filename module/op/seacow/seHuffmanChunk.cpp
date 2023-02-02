#include "seHuffmanChunk.h"
#include "seHuffmanBlock.h"
#include <util/ioutil.h>

namespace msdb
{
namespace core
{

//////////////////////////////
// seHuffmanChunkType
//
seHuffmanChunkType::seHuffmanChunkType(const dataType& type)
	: chunkType(buildFactory(seHuffmanChunkFactoryBuilder(), type))
{
}

template <typename Ty_>
std::vector<std::vector<iFixedHuffmanCoder*>> seHuffmanChunk<Ty_>::fixedHuffmanCoders =
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
}		// core
}		// msdb