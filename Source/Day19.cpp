#include "Days.h"
#include "AdventUtils.h"
#include <list>
#include <map>
#include <regex>
#include <vector>

const regex RX_RULE(R"((\d+): (.+))");

struct Rule
{
    Rule()
        : SequenceOptions({})
    {
    }

    Rule(vector<list<string>> inSequenceOptions)
        : SequenceOptions(inSequenceOptions)
    {
    }

    /** An array of possible sequences this Rule allows (e.g. 1,2,3 | a,b,c | 1,1,b) */
    vector<list<string>> SequenceOptions;
};

// Mapping of Rule Number to the Rule
static map<int, Rule> MasterRuleMap = {};

bool StringMatchesSequence(const string& str, list<string> sequence)
{
    if (sequence.size())
    {
        string sequenceIt = sequence.front();
        sequence.pop_front();
        if (IsNumber(sequenceIt))
        {
            // Sequence should be a rule
            int ruleNum = stoi(sequenceIt);
            const Rule& rule = MasterRuleMap[ruleNum];

            // Iterate by copy
            for (list<string> ruleSequence : rule.SequenceOptions)
            {
                // Recursively call this function, inserting the sequence option into the greater "parent" sequence
                ruleSequence.insert(ruleSequence.end(), sequence.begin(), sequence.end());
                bool bStringMatch = StringMatchesSequence(str, ruleSequence);
                if (bStringMatch)
                {
                    // Early out on success
                    return true;
                }
            }
        }
        else
        {
            // Sequence should be a string, try matching directly
            size_t strLen = sequenceIt.length();
            if (str.substr(0, strLen) == sequenceIt)
            {
                // Recursively call this function on the remaining sequence with the remaining string
                string newStr = str.substr(strLen);
                return StringMatchesSequence(newStr, sequence);
            }
            else
            {
                return false;
            }
        }
    }
    else if (str.empty())
    {
        // Empty string and empty sequence means they've matched!
        return true;
    }
    return false;
}

template <>
void Run<Day19>(Part part, istream& is, ostream& os)
{
    vector<string> messages = {};
	
    string arg;
	while (getline(is, arg))
	{
		if (!arg.empty())
		{
			smatch match;
			if (regex_search(arg, match, RX_RULE))
			{
				// Found a rule, parse line manually to detect alphanumerics or pipe character
				int ruleNum = stoi(match.str(1));
				string ruleStr = match.str(2);
				
				string buf = "";
				list<string> sequence;
				vector<list<string>> sequenceOptions;
				for (size_t i = 0; i < ruleStr.size(); ++i)
				{
					char c = ruleStr[i];
					if (isalnum(c))
					{
						buf.push_back(c);
					}
					
					if (!buf.empty() && (c == ' ' || i == ruleStr.size() - 1))
					{
						sequence.push_back(buf);
						buf.clear();
					}
					
					if (c == '|' || i == ruleStr.size() - 1)
					{
						sequenceOptions.push_back(sequence);
						sequence.clear();
					}
				}
                MasterRuleMap[ruleNum] = Rule(sequenceOptions);
			}
			else
			{
                messages.push_back(arg);
			}
		}
	}

    if (part == Part02)
    {
        // Part 2 input override
        MasterRuleMap[8] = Rule({{"42"}, { "42", "8" }});
        MasterRuleMap[11] = Rule({{"42", "31"}, { "42", "11", "31" }});
    }
    
    int matchCount = 0;
    for (const string& message : messages)
    {
        bool bStringMatch = StringMatchesSequence(message, {"0"});
        matchCount += static_cast<int>(bStringMatch);
        // os << arg << ": " << (bStringMatch ? "True" : "False") << endl;
    }
	os << "Match Count: " << matchCount << endl;
}