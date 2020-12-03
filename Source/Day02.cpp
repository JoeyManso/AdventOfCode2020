#include "Days.h"
#include <regex>

using namespace std;

const regex RX_PASSWORD_DATA_PATTERN(R"((\d+)-(\d+) ([a-z]): (\w+))");

struct PasswordData
{
	size_t FirstNum = 0;
	size_t SecondNum = 0;
	char RequiredChar;
	string Password;

	/** Validates that the Required char count is inclusively between the first and second number */
	bool IsPasswordValid1() const
	{
		int requiredCharCount = 0;
		for (const char c : Password)
		{
			if (c == RequiredChar)
			{
				++requiredCharCount;
			}
		}
		return requiredCharCount >= FirstNum && requiredCharCount <= SecondNum;
	}

	/** Validates that the Required char count has exactly one entry at either the first or second number (not 0 based) */
	bool IsPasswordValid2() const
	{
		return (Password[FirstNum-1] == RequiredChar) ^ (Password[SecondNum-1] == RequiredChar);
	}
};

PasswordData ArgToPasswordData(string _arg)
{	
	smatch match;
	regex_search(_arg, match, RX_PASSWORD_DATA_PATTERN);
	
	PasswordData passwordData;
	passwordData.FirstNum = stoi(match.str(1));
	passwordData.SecondNum = stoi(match.str(2));
	passwordData.RequiredChar = match.str(3)[0];
	passwordData.Password = match.str(4);
	return passwordData;
}

template <>
void Run<Day02>(Part part, istream& is, ostream& os)
{
	os << "Parsing Passwords..." << endl;
	string arg;
	int totalPasswords = 0, validPasswords = 0;
	while(getline(is, arg))
	{
		const PasswordData passwordData = ArgToPasswordData(arg);
		if(part == Part01 && passwordData.IsPasswordValid1())
		{
			++validPasswords;
		}
		else if(part == Part02 && passwordData.IsPasswordValid2())
		{
			++validPasswords;
		}
		++totalPasswords;
	}
	os << "Total Passwords: " << totalPasswords << endl;
	os << "Valid Passwords: " << validPasswords << endl;
}