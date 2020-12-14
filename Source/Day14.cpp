#include "Days.h"
#include <map>
#include <regex>
#include <vector>

regex RX_MASK(R"(mask = (\w+))");
regex RX_MEM(R"(mem\[(\d+)\] = (\d+))");

const unsigned long long ONE = 1;

template <>
void Run<Day14>(Part part, istream& is, ostream& os)
{
	unsigned long long maskOr = 0;
    unsigned long long maskAnd = ~maskOr;
    unsigned long long maskEither = 0;
	map<unsigned long long, unsigned long long> memValuesMap = {};
	string arg;
	while (getline(is, arg))
	{
		smatch matchMask, matchMem;
		if (regex_search(arg, matchMask, RX_MASK))
		{
			// Construct masks, reverse order from the input
			string maskStr = matchMask.str(1);
            maskOr = 0;
            maskAnd = ~maskOr;
            maskEither = 0;
			for (unsigned long long i = 0; i < maskStr.size(); ++i)
			{
				unsigned long long idx = maskStr.size() - 1 - i;
				switch (maskStr[idx])
				{
					case '0':
						maskAnd -= (ONE << i);
						break;
					case '1':
						maskOr += (ONE << i);
						break;
					default:
						maskEither += (ONE << i);
						break;
				}
			}
		}
		else if (regex_search(arg, matchMem, RX_MEM))
		{
			unsigned long long address = stoull(matchMem.str(1));
			unsigned long long value = stoull(matchMem.str(2));

			if (part == Part01)
			{
				// Modify value with the mask and assign to the specified address
				unsigned long long maskedValue = ((value & maskAnd) | maskOr);
				memValuesMap[address] = maskedValue;
			}
			else if(part == Part02)
			{
				vector<unsigned long long> maskedAddresses;
				maskedAddresses.push_back((address | maskOr) & (~maskEither));
				for (unsigned long long i = 0; i < 36; ++i)
				{
					// Check each bit for "wild", when found push back possible iterations with that bit
					if (((maskEither >> i) & ONE) > 0)
					{
						size_t jMax = maskedAddresses.size();
						for (size_t j = 0; j < jMax; j++)
						{
							maskedAddresses.push_back(maskedAddresses[j] + (ONE << i));
						}
					}
				}

				// Write out the value to each potential address
				for (const auto& maskedAddress : maskedAddresses)
				{
					memValuesMap[maskedAddress] = value;
				}
			}
		}
	}
	
	unsigned long long sum = 0;
	for (const auto& memValuesPair : memValuesMap)
	{
		sum += memValuesPair.second;
	}
	os << "Memory values count: " << memValuesMap.size() << endl;
	os << "Memory values sum: " << sum << endl;
}