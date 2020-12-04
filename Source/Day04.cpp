#include "Days.h"
#include "AdventMath.h"
#include <regex>
#include <set>
#include <vector>

const vector<regex> RX_REQUIRED_FIELDS = 
{
	regex(R"((byr):(#?\w+))"), //(Birth Year)
	regex(R"((iyr):(#?\w+))"), //(Issue Year)
	regex(R"((eyr):(#?\w+))"), //(Expiration Year)
	regex(R"((hgt):(#?\w+))"), //(Height)
	regex(R"((hcl):(#?\w+))"), //(Hair Color)
	regex(R"((ecl):(#?\w+))"), //(Eye Color)
	regex(R"((pid):(#?\w+))"), //(Passport ID)
};

template <>
void Run<Day04>(Part part, istream& is, ostream& os)
{
	string arg;
	set<string> validatedFields = {};
	int countValidPassports = 0;
	int countTotalPassports = 0;
	while (getline(is, arg))
	{
		for (const regex rxRequiredField : RX_REQUIRED_FIELDS)
		{
			smatch matchArg;
			regex_search(arg, matchArg, rxRequiredField);
			if (matchArg.size())
			{
				const string field = matchArg.str(1);
				if (part == Part01)
				{
					// Part one only requires that each field exists
					validatedFields.emplace(field);
				}
				else if (part == Part02)
				{
					// Part two requires further refinement, validating the value of the field
					const regex rxYear = regex(R"((\d{4}))");
					const regex rxHgt = regex(R"((\d{2,3})(cm|in))");
					const regex rxHcl = regex(R"((#[0-9a-f]{6}))");
					const regex rxEcl = regex(R"((amb|blu|brn|gry|grn|hzl|oth))");
					const regex rxPid = regex(R"((\d{9}))");

					bool isValidatedField = false;
					const string value = matchArg.str(2);
					if (field == "byr")
					{
						smatch matchByr;
						regex_search(value, matchByr, rxYear);
						if(matchByr.size())
						{
							isValidatedField = InRangeInclusive(stoi(matchByr.str()), 1920, 2002);
						}
					}
					else if (field == "iyr")
					{
						smatch matchIyr;
						regex_search(value, matchIyr, rxYear);
						if(matchIyr.size())
						{
							isValidatedField = InRangeInclusive(stoi(matchIyr.str()), 2010, 2020);
						}
					}
					else if (field == "eyr")
					{
						smatch matchEyr;
						regex_search(value, matchEyr, rxYear);
						if(matchEyr.size())
						{
							isValidatedField = InRangeInclusive(stoi(matchEyr.str()), 2020, 2030);
						}
					}
					else if (field == "hgt")
					{
						smatch matchHgt;
						regex_search(value, matchHgt, rxHgt);
						if(matchHgt.size())
						{
							int height = stoi(matchHgt.str(1));
							string heightType = matchHgt.str(2);
							if (heightType == "cm")
							{
								isValidatedField = InRangeInclusive(height, 150, 193);
							}
							else if (heightType == "in")
							{
								isValidatedField = InRangeInclusive(height, 59, 76);
							}	
						}											
					}
					else if (field == "hcl")
					{
						isValidatedField = regex_match(value, rxHcl);
					}
					else if (field == "ecl")
					{
						isValidatedField = regex_match(value, rxEcl);
					}
					else if (field == "pid")
					{
						isValidatedField = regex_match(value, rxPid);
					}
					if (isValidatedField)
					{
						validatedFields.emplace(field);
					}
				}
			}
		}
		
		// Newline means the password has completed parsing, check it for validity
		if (arg.length() == 0 || is.eof())
		{
			if (validatedFields.size() >= RX_REQUIRED_FIELDS.size())
			{
				++countValidPassports;
			}
			++countTotalPassports;
			validatedFields.clear();
		}
	}
		
	os << "Total Passports: " << countTotalPassports << endl;
	os << "Valid Passports: " << countValidPassports << endl;
}