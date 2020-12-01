#include "Days.h"
#include <vector>

using namespace std;

static const int GOAL_SUM = 2020;

/** Recursively searches the given inputNumbers to find a combination of requiredCanidates size that reaches GOAL_SUM */
bool FindGoalSum(const vector<int>& inputNumbers, vector<int>& candidateNumbers, const int index, const int requiredCandidates)
{
	bool bReachedGoalSum = false;
	if (candidateNumbers.size() == requiredCandidates)
	{
		// Reached candidates target, check if the sum matches the goal
		int candidateSum = 0;
		for (const int candidateNumber : candidateNumbers)
		{
 			candidateSum += candidateNumber;
		}
		bReachedGoalSum = candidateSum == GOAL_SUM;
	}
	else
	{
		for (int i = index; i < inputNumbers.size(); ++i)
		{
			candidateNumbers.push_back(inputNumbers[i]);
			if (FindGoalSum(inputNumbers, candidateNumbers, i, requiredCandidates))
			{
				break;
			}
			else
			{
				candidateNumbers.pop_back();
			}
		}
	}
	return bReachedGoalSum;
}

void SolveForCandidates(const vector<int>& inputNumbers, ostream& os, const int requiredCandidates)
{
	// Iterate through the given numbers, using i as a starting recursive search point (offset from max depth) until a solution is found
	vector<int> candidateNumbers;
	for (int i = 0; i < inputNumbers.size() - requiredCandidates; ++i)
	{
		if (FindGoalSum(inputNumbers, candidateNumbers, i, requiredCandidates))
		{
			int prod = 0;
			string sumStr = "", prodStr = "";
			for (int i = 0; i < candidateNumbers.size(); ++i)
			{
				int candidateNumber = candidateNumbers[i];
				prod = (i == 0 ? candidateNumber : prod * candidateNumber);
				if (i == candidateNumbers.size() - 1)
				{
					sumStr += to_string(candidateNumber) + " = " + to_string(GOAL_SUM);
					prodStr += to_string(candidateNumber) + " = " + to_string(prod);
				}
				else
				{
					sumStr += to_string(candidateNumber) + " + ";
					prodStr += to_string(candidateNumber) + " * ";
				}
			}
			os << sumStr << endl;
			os << prodStr << endl;
			break;
		}
	}
}

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
		SolveForCandidates(inputNumbers, os, 2);
	}
	else if(part == Part02)
	{		
		SolveForCandidates(inputNumbers, os, 3);
	}
}