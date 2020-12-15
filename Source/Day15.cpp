#include "Days.h"
#include <list>
#include <map>
#include <regex>

const regex RX_NUM(R"((\d+))");

template <>
void Run<Day15>(Part part, istream& is, ostream& os)
{
	const int turnsMax = part == Part01 ? 2020 : 30000000;
	
	// Running map of a number and the last turns it was "spoken"
	map<int, list<int>> spokenNumberTurnsMap = {};
	
	// The "spoken" number
	int spokenNumber = 0;

	// The current turn
	int turnIdx = 1;
	
	os << "\"Speaking\" " << turnsMax << " numbers..." << endl;
	string arg;
	while (getline(is, arg))
	{
		// Read starting numbers, this assumes there is only one line...
		for (sregex_token_iterator it = {arg.begin(), arg.end(), RX_NUM, 1};
			it != sregex_token_iterator();
			it = next(it))
		{
			spokenNumber = stoi(*it);
			spokenNumberTurnsMap[spokenNumber] = list<int>{turnIdx};
			++turnIdx;
		}
	}
	
	for (turnIdx; turnIdx <= turnsMax; ++turnIdx)
	{
		// Retrieve entry for last spoken number
		const list<int>& spokenTurns = spokenNumberTurnsMap[spokenNumber];
		
		// Determine new spoken number:
		// If last spoken number was heard for the first time (only 1 entry), next spoken number is 0
		// Else, the next spoken number is the difference of the previous turns
		if (spokenTurns.size() > 1)
		{
			spokenNumber = spokenTurns.back() - spokenTurns.front();
		}
		else
		{
			spokenNumber = 0;
		}

		// Update (or add) entry for the "new" spoken number, including this turn
		// Trim list to the last two turns
		list<int>& newSpokenTurns = spokenNumberTurnsMap[spokenNumber];
		newSpokenTurns.push_back(turnIdx);
		while (newSpokenTurns.size() > 2)
		{
			newSpokenTurns.pop_front();
		}
	}
	os << "The last \"Spoken\" number: " << spokenNumber << endl;
}