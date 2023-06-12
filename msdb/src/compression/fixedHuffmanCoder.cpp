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

	double laplaceBack = std::max((double)bits - 3.0, (double)1.0);
	double laplaceSd = (double)pow(2.0, laplaceBack) / 1.5;
	boost::math::laplace_distribution<double, boost::math::policies::policy<>> ndNarrow(mean, laplaceSd);
	BOOST_LOG_TRIVIAL(debug) << "Freq==>laplaceSdExp: " << laplaceSd << ", range: " << range << ", precision: " << precision << std::endl;

	freq[0] = (boost::math::pdf(ndNarrow, 0.0)) * precision;
	freq[range] = 1;

	for (size_t x = 1; x < range; ++x)
	{
		freq[x] = (boost::math::pdf(ndNarrow, (double)x)) * precision;
		if (freq[x] == 0)
		{
			freq[x] = 1;
		}
		freq[x + range] = freq[x];
		BOOST_LOG_TRIVIAL(trace) << "[" << x << "]: " << boost::math::pdf(ndNarrow, (double)x) << "=" << freq[x];
	}

	//////////////////////////////////
	//size_t range = pow(2, bits - 1);
	//size_t precision = 500000 / pow(2, 8 - bits);
	//double mean = 0;

	//double back = std::max((double)bits - 3.0, (double)1.0);
	//double sdExp = (double)pow(2.0, back);
	//boost::math::normal_distribution<double, boost::math::policies::policy<>> ndNarrow(mean, sdExp);
	//BOOST_LOG_TRIVIAL(debug) << "Freq==>sdExp: " << sdExp << ", range: " << range << ", precision: " << precision << std::endl;

	//freq[0] = (boost::math::pdf(ndNarrow, 0.0)) * precision;
	//freq[range] = 1;

	//for (size_t x = 1; x < range; ++x)
	//{
	//	freq[x] = (boost::math::pdf(ndNarrow, (double)x)) * precision;
	//	if (freq[x] == 0)
	//	{
	//		freq[x] = 1;
	//	}
	//	freq[x + range] = freq[x];
	//	BOOST_LOG_TRIVIAL(trace) << "[" << x << "]: " << boost::math::pdf(ndNarrow, (double)x) << "=" << freq[x];
	//}

	//////////////////////////////////
	//size_t range = pow(2, bits - 1);
	//size_t precision = 500000 / pow(2, 8 - bits);
	//double mean = 0;
	//double back = std::max((double)bits - 3.0, (double)1.0);
	//double sdExp = (double)pow(2.0, back);

	//boost::math::normal_distribution<double, boost::math::policies::policy<>> ndNarrow(mean, sdExp);
	//BOOST_LOG_TRIVIAL(debug) << "Freq==>sdExp: " << sdExp << ", range: " << range << ", precision: " << precision << std::endl;
	// 
	//freq[0] = (boost::math::pdf(ndNarrow, 0.0)) * precision;
	//freq[range] = 1;
	//for (size_t x = 1; x < range; ++x)
	//{
	//	freq[x] = (boost::math::pdf(ndNarrow, (double)x)) * precision;
	//	if (freq[x] == 0)
	//	{
	//		freq[x] = 1;
	//	}
	//	freq[x + range] = freq[x];
	//	//BOOST_LOG_TRIVIAL(trace) << "[" << x << "]: " << boost::math::pdf(ndNarrow, (double)x) << "+" << boost::math::pdf(ndWide, (double)x) << "=" << freq[x];
	//}

	//////////////////////////
	//size_t range = pow(2, bits - 1);
	////size_t half = ceil(range / 2.0);
	//size_t precision = 100000;
	//double mean = 0;
	////double sdNarrow = std::max({ (size_t)bits - 1, (size_t)1 });
	//double sdNarrow = (double)bits / 2.0;
	//double sdWide = sdNarrow * 10;

	//boost::math::normal_distribution<double, boost::math::policies::policy<>> ndNarrow(mean, sdNarrow);
	//boost::math::normal_distribution<double, boost::math::policies::policy<>> ndWide(mean, sdWide);

	//freq[0] = (boost::math::pdf(ndNarrow, 0.0) + boost::math::pdf(ndWide, 0.0)) * precision;
	//freq[range] = 1;
	//BOOST_LOG_TRIVIAL(trace) << "Freq==>narrow: " << sdNarrow << ", wide: " << sdWide << ", range: " << range;;
	//for (size_t x = 1; x < range; ++x)
	//{
	//	freq[x] = (boost::math::pdf(ndNarrow, (double)x) + boost::math::pdf(ndWide, (double)x)) * precision;
	//	if (freq[x] == 0)
	//	{
	//		freq[x] = 1;
	//	}
	//	freq[x + range] = freq[x];
	//	BOOST_LOG_TRIVIAL(trace) << "[" << x << "]: " << boost::math::pdf(ndNarrow, (double)x) << "+" << boost::math::pdf(ndWide, (double)x) << "=" << freq[x];
	//}
}
}		// core
}		// msdb
