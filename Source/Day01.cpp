#include "Days.h"
#include <vector>

using namespace std;

static const int GOAL_SUM = 2020;

template <>
void Run<Day01>(Part part, istream& is, ostream& os)
{
	string arg;
	vector<int> inputNumbers;
	while(getline(is, arg))
	{
		inputNumbers.push_back(stoi(arg));
	}
	os << "Found " << inputNumbers.size() << " numbers" << endl;

	if(part == Part01)
	{		
		for (int i = 0; i < inputNumbers.size(); ++i)
		{
			for (int j = i; j < inputNumbers.size(); ++j)
			{
				if (inputNumbers[i] + inputNumbers[j] == GOAL_SUM)
				{
					os << inputNumbers[i] << " + " << inputNumbers[j] << " = " << GOAL_SUM << endl;
					os << inputNumbers[i] << " * " << inputNumbers[j] << " = " << inputNumbers[i] * inputNumbers[j] << endl;
					return;
				}
			}
		}
	}
	else if(part == Part02)
	{		
		for (int i = 0; i < inputNumbers.size(); ++i)
		{
			for (int j = i; j < inputNumbers.size(); ++j)
			{
				for (int k = j; k < inputNumbers.size(); ++k)
				{
					if (inputNumbers[i] + inputNumbers[j] + inputNumbers[k] == GOAL_SUM)
					{
						os << inputNumbers[i] << " + " << inputNumbers[j] << " + " << inputNumbers[k] << " = " << GOAL_SUM << endl;
						os << inputNumbers[i] << " * " << inputNumbers[j] << " + " << inputNumbers[k] << " = " << inputNumbers[i] * inputNumbers[j] * inputNumbers[k] << endl;
						return;
					}
				}
			}
		}
	}
}