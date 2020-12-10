#include "Days.h"
#include <algorithm>
#include <map>
#include <list>
#include <vector>

/** Recursively calculates all possible arrangements of the given adapters */
unsigned long long GetTotalArrangements(const vector<int>& adapters, int previousAdapter, size_t idxStart)
{
	// Memoization - https://en.wikipedia.org/wiki/Memoization
	static map<size_t, unsigned long long> calculatedArrangements = {};

	unsigned long long totalArrangements = 0;
	for (size_t i = idxStart; i < adapters.size(); ++i)
	{
		int currentAdapter = adapters[i];
		int delta = currentAdapter - previousAdapter;
		if (delta <= 3)
		{
			if (i == adapters.size() - 1)
			{
				++totalArrangements;
			}
			else
			{
				// Cache calculated answers from each index to avoid calculating them again
				if (calculatedArrangements.find(i) == calculatedArrangements.end())
				{
					calculatedArrangements.emplace(i, GetTotalArrangements(adapters, currentAdapter, i + 1));
				}
				totalArrangements += calculatedArrangements[i];
			}
		}
		else
		{
			break;
		}
	}
	return totalArrangements;
}

template <>
void Run<Day10>(Part part, istream& is, ostream& os)
{
	// Collect adapters for processing, starting with the default device voltage (0)
	vector<int> adapters = {0};
	string arg;
	while (getline(is, arg))
	{
		int adapter = stoi(arg);
		adapters.push_back(adapter);
	}

	// Sort adapters, final "adapter" is the outlet, rated +3 from max
	sort(adapters.begin(), adapters.end());
	adapters.push_back(adapters[adapters.size() - 1] + 3);

	if(part == Part01)
	{
		// Adapters are sorted, collect the differences between each in the arrangement
		map<int, int> deltaDiffMap = {};
		for (size_t i = 0; i < adapters.size() - 1; ++i)
		{
			int delta = adapters[i + 1] - adapters[i];
			if (deltaDiffMap.find(delta) == deltaDiffMap.end())
			{
				deltaDiffMap.emplace(delta, 0);
			}
			++deltaDiffMap[delta];
		}

		int multipliedDifferences = 1;
		for (const auto& deltaDiffPair : deltaDiffMap)
		{
			os << deltaDiffPair.second << " differences of " << deltaDiffPair.first << " jolts" << endl;
			multipliedDifferences *= deltaDiffPair.second;
		}
		os << "Multiplied differences: " << multipliedDifferences << endl;
	}
	else if(part == Part02)
	{
		unsigned long long arrangements = GetTotalArrangements(adapters, adapters[0], 1);
		os << "Total possible arrangements: " << arrangements << endl;
	}
}