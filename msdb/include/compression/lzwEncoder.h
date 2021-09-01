#pragma once
#ifndef _MSDB_LZWENCODER_H_
#define _MSDB_LZWENCODER_H_

#include <pch.h>
#include <io/bitstream.h>

namespace msdb
{
namespace core
{
template <typename lzwCodeType>
class lzwCoder
{
public:
	static const lzwCodeType dms{ std::numeric_limits<lzwCodeType>::max() };

public:
	lzwCoder()
	{

	}

	~lzwCoder()
	{

	}

	void encode(bstream& out, const unsigned char* in, const size_t len)
	{
		std::map<std::pair<lzwCodeType, char>, lzwCodeType> dictionary;

		// "named" lambda function, used to reset the dictionary to its initial contents
		const auto reset_dictionary = [&dictionary]
		{
			dictionary.clear();

			const long int minc = std::numeric_limits<unsigned char>::min();
			const long int maxc = std::numeric_limits<unsigned char>::max();

			for (long int c = minc; c <= maxc; ++c)
			{
				// to prevent Undefined Behavior, resulting from reading and modifying
				// the dictionary object at the same time
				const lzwCodeType dictionary_size = dictionary.size();

				dictionary[{lzwCoder<lzwCodeType>::dms, static_cast<unsigned char> (c)}] = dictionary_size;
			}
		};

		reset_dictionary();

		lzwCodeType i{ lzwCoder<lzwCodeType>::dms }; // Index
		unsigned char c;

		out << setw(sizeof(lzwCodeType) * CHAR_BIT);

		for (size_t pos = 0; pos < len; ++pos)
		{
			c = in[pos];

			// dictionary's maximum size was reached
			if (dictionary.size() == lzwCoder<lzwCodeType>::dms)
			{
				reset_dictionary();
			}

			if (dictionary.count({ i, c }) == 0)
			{
				// to prevent Undefined Behavior, resulting from reading and modifying
				// the dictionary object at the same time
				const lzwCodeType dictionary_size = dictionary.size();

				dictionary[{i, c}] = dictionary_size;
				out << i;
				//compressed.write(reinterpret_cast<const char*> (&i), sizeof(lzwCodeType));
				i = dictionary.at({ lzwCoder<lzwCodeType>::dms, c });
			}
			else
			{
				i = dictionary.at({ i, c });
			}
		}

		if (i != lzwCoder<lzwCodeType>::dms)
		{
			out << i;
			//compressed.write(reinterpret_cast<const char*> (&i), sizeof(lzwCodeType));
		}
	}

	void decode(unsigned char* out, const size_t len, bstream& in)
	{
		std::vector<std::pair<lzwCodeType, unsigned char>> dictionary;

		// "named" lambda function, used to reset the dictionary to its initial contents
		const auto reset_dictionary = [&dictionary]
		{
			dictionary.clear();
			dictionary.reserve(lzwCoder<lzwCodeType>::dms);

			const long int minc = std::numeric_limits<unsigned char>::min();
			const long int maxc = std::numeric_limits<unsigned char>::max();

			for (long int c = minc; c <= maxc; ++c)
			{
				dictionary.push_back({ lzwCoder<lzwCodeType>::dms, static_cast<unsigned char> (c) });
			}
		};

		const auto rebuild_string = [&dictionary](lzwCodeType k) -> std::vector<unsigned char>
		{
			std::vector<unsigned char> s; // String

			while (k != lzwCoder<lzwCodeType>::dms)
			{
				s.push_back(dictionary.at(k).second);
				k = dictionary.at(k).first;
			}

			std::reverse(s.begin(), s.end());
			return s;
		};

		reset_dictionary();

		lzwCodeType i{ lzwCoder<lzwCodeType>::dms }; // Index
		lzwCodeType k; // Key

		in >> setw(sizeof(lzwCodeType) * CHAR_BIT);
		size_t outSeq = 0;
		while(!in.eof())
		//while (compressed.read(reinterpret_cast<char*> (&k), sizeof(lzwCodeType)))
		{
			in >> k;

			// dictionary's maximum size was reached
			if (dictionary.size() == lzwCoder<lzwCodeType>::dms)
				reset_dictionary();

			if (k > dictionary.size())
			{
				throw std::runtime_error("invalid compressed code");
			}

			std::vector<unsigned char> s; // String
			if (k == dictionary.size())
			{
				dictionary.push_back({ i, rebuild_string(i).front() });
				s = rebuild_string(k);
			}
			else
			{
				s = rebuild_string(k);

				if (i != lzwCoder<lzwCodeType>::dms)
					dictionary.push_back({ i, s.front() });
			}

			for (int j = 0; j < s.size(); ++j, ++outSeq)
			{
				out[outSeq] = s[j];
			}
			i = k;
		}

		if (!in.eof())
		{
			throw std::runtime_error("corrupted compressed file");
		}
	}

private:

};
}		// core
}		// msdb
#endif	// _MSDB_LZWENCODER_H_
