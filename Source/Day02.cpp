#include "Days.h"
using namespace std;

struct PasswordData
{
	string Password;
	char RequiredChar;
	size_t FirstNum = 0;
	size_t SecondNum = 0;

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
	PasswordData passwordData;
	size_t idxDash = _arg.find('-');
	size_t idxColon = _arg.find(':');
	size_t idxRequiredChar = idxColon - 1;
	size_t maxCharLen = (idxRequiredChar - 1) - (idxDash + 1);

	passwordData.Password = _arg.substr(idxColon + 2);
	passwordData.RequiredChar = _arg[idxRequiredChar];
	passwordData.FirstNum = stoi(_arg.substr(0,idxDash));
	passwordData.SecondNum = stoi(_arg.substr(idxDash + 1, maxCharLen));

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