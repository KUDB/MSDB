#include <pch.h>
#include <compression/fixedHuffmanCoder.h>

namespace msdb
{
namespace core
{
void iFixedHuffmanCoder::initFreq(std::vector<size_t>& freq, size_t bits)
{
	size_t range = pow(2, bits - 1);
	size_t precision = 500000 / pow(2, 8 - bits);
	double mean = 0;
	//double back = std::max({ (bits - 3.0), 1.0 });
	double back = bits - 3.0;
	double sdExp = (double)pow(2.0, back);

	boost::math::normal_distribution<double, boost::math::policies::policy<>> ndNarrow(mean, sdExp);

	freq[0] = (boost::math::pdf(ndNarrow, 0.0)) * precision;
	freq[range] = 1;
	BOOST_LOG_TRIVIAL(debug) << "Freq==>sdExp: " << sdExp << ", range: " << range << ", precision: " << precision << std::endl;
	for (size_t x = 1; x < range; ++x)
	{
		freq[x] = (boost::math::pdf(ndNarrow, (double)x)) * precision;
		if (freq[x] == 0)
		{
			freq[x] = 1;
		}
		freq[x + range] = freq[x];
		//BOOST_LOG_TRIVIAL(trace) << "[" << x << "]: " << boost::math::pdf(ndNarrow, (double)x) << "+" << boost::math::pdf(ndWide, (double)x) << "=" << freq[x];
	}
}
}		// core
}		// msdb