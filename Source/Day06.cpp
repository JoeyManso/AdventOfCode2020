#include "Days.h"
#include <map>

template <>
void Run<Day06>(Part part, istream& is, ostream& os)
{
	string arg;
	map<char, size_t> groupAnswersMap = {};
	size_t numPeopleInGroup = 0;
	size_t numGroups = 0;
	size_t numAnswers = 0;

	os << "Reading Answers..." << endl;
	while (getline(is, arg))
	{	
		if (arg.length())
		{
			// Read answer line
			for (const char c : arg)
			{
				// Construct map of answer letter to group response count
				if (groupAnswersMap.find(c) == groupAnswersMap.end())
				{
					groupAnswersMap.emplace(c, 0);
				}
				++groupAnswersMap[c];
			}
			++numPeopleInGroup;
		}
		if (arg.length() == 0 || is.eof())
		{
			// Newline means the questions have completed parsing, count the total shared answers
			++numGroups;
			for (const auto& groupAnswersPair : groupAnswersMap)
			{
				// Grab the total answers for each key
				size_t groupAnswers = groupAnswersPair.second;

				// Part 1 counts any entry for an answer
				// Part 2 only counts if all respondents answered
				if ((part == Part01 && groupAnswers > 0)
				|| (part == Part02 && groupAnswers == numPeopleInGroup))
				{
					++numAnswers;
				}
			}
			groupAnswersMap.clear();
			numPeopleInGroup = 0;
		}
	}
		
	os << "Total Groups: " << numGroups << endl;
	os << "Total Answers: " << numAnswers << endl;
}