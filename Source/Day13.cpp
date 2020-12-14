#include "Days.h"
#include <numeric>
#include <regex>
#include <vector>

const regex RX_NUM(R"((\w+))");

bool IsNumber(const string& s)
{
    return find_if(s.begin(),  s.end(), [](unsigned char c) { return !::isdigit(c); }) == s.end();
}

/** Returns the next bus time starting from the given departure */
long long GetNextBusTime(long long departTime, int busId)
{
	long long nextBusTime = ((departTime / busId) * busId);
	if (nextBusTime < departTime)
	{
		nextBusTime += busId;
	}
	return nextBusTime;
}

template <>
void Run<Day13>(Part part, istream& is, ostream& os)
{
	string arg;
	vector<string> args = {};
	while (getline(is, arg))
	{
		args.push_back(arg);
	}
	
	// Retrieve all bus IDs
	vector<int> busIds = {};
	sregex_token_iterator it = {arg.begin(), arg.end(), RX_NUM, 1};
	while(it != sregex_token_iterator())
	{
		if (IsNumber(*it))
		{
			busIds.push_back(stoi(*it));
		}
		else
		{
			busIds.push_back(0);
		}
		it = next(it);
	}

	if(part == Part01)
	{
		// Grab initial depart time
		long long departTime = stoi(args[0]);
		os << "Depart Time: " << departTime << endl;

		int bestBusId = 0;
		long long bestBusTime = LLONG_MAX;
		for (int busId : busIds)
		{
			if (busId > 0)
			{
				long long busTime = GetNextBusTime(departTime, busId);
				if (busTime < bestBusTime)
				{
					bestBusTime = busTime;
					bestBusId = busId;
				}
			}
		}
		long long waitTime = bestBusTime - departTime;
		os << "Best Bus ID: " << bestBusId << endl;
		os << "Best Bus Time: " << bestBusTime << endl;
		os << "Wait Time: " << waitTime << endl;
		os << "Multiplied: " << bestBusId * waitTime << endl;
	}
	else if(part == Part02)
	{
		os << "Parsing timestamps..." << endl;
		
		// Iterate over (actual) bus times, increasing the departure by "departureInc" until it leaves at the correct time
		// "departureInc" is calculated the Least Common Denominator of itself and the current busId, to ensure future found departure times will continue to work
		long long departureInc = busIds[0];
		long long departTime = 0;
		for (size_t i = 0; i < busIds.size(); ++i)
		{
			int busId = busIds[i];
			if (busId > 0)
			{
				while (((departTime + i) % busId) != 0)
				{
					departTime += departureInc;
				}
				departureInc = lcm(departureInc, busIds[i]);
			}
		}
		os << "Magic Depart Time: " << departTime << endl;
	}
}