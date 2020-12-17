#include "Days.h"
#include "AdventMath.h"
#include <map>
#include <regex>
#include <set>
#include <vector>

const regex RX_FIELD_RANGES(R"((.+): (\d+)-(\d+) or (\d+)-(\d+))");
const regex RX_NUM(R"((\d+))");

struct FieldData
{
	FieldData(const smatch& validMatch)
	{
		fieldName = validMatch.str(1);
		fieldRange1 = {stoi(validMatch.str(2)), stoi(validMatch.str(3))};
		fieldRange2 = {stoi(validMatch.str(4)), stoi(validMatch.str(5))};
	}

	bool IsValidTicketNumber(int ticketNumber) const
	{
		return InRangeInclusive(ticketNumber, fieldRange1.first, fieldRange1.second) || InRangeInclusive(ticketNumber, fieldRange2.first, fieldRange2.second);
	}

	string fieldName;
	pair<int, int> fieldRange1;
	pair<int, int> fieldRange2;
};

template <>
void Run<Day16>(Part part, istream& is, ostream& os)
{
	vector<FieldData> fieldDataArray = {};
	vector<int> myTicket = {};
	vector<vector<int>> nearbyTickets = {};
	int invalidTicketValueSum = 0;
	
	os << "Parsing ticket data..." << endl;
	string arg;
	while (getline(is, arg))
	{
		smatch match;
		if (regex_search(arg, match, RX_FIELD_RANGES))
		{
			// Collect all fields
			fieldDataArray.push_back(FieldData(match));
		}
		else
		{
			// Collect ticket numbers
			size_t i = 0;
			vector<int> ticketNumbers;
			bool isValidTicket = true;
			for (sregex_token_iterator it = {arg.begin(), arg.end(), RX_NUM, 1};
				it != sregex_token_iterator();
				it = next(it))
			{
				// Check if this ticket number is valid against ANY field
				int ticketNumber = stoi(*it);
				bool isValidNumber = false;
				for (const FieldData& fieldData : fieldDataArray)
				{
					isValidNumber |= fieldData.IsValidTicketNumber(ticketNumber);
				}
				ticketNumbers.push_back(ticketNumber);
				if (!isValidNumber)
				{
					invalidTicketValueSum += ticketNumber;
					isValidTicket = false;
				}
			}

			if (ticketNumbers.size() > 0)
			{
				if (myTicket.size() == 0)
				{
					// The first ticket is our ticket!
					myTicket = ticketNumbers;
				}
				else if (isValidTicket)
				{
					// All other tickets need to be valid
					nearbyTickets.push_back(ticketNumbers);
				}
			}
		}
	}

	if (part == Part01)
	{
		os << "Invalid Ticket Value Sum: " << invalidTicketValueSum << endl;
	}
	else if (part == Part02)
	{	
		// Mapping of field name to possible valid column indices
		os << "Ordering Columns... " << endl;
		map<string, set<size_t>> possibleFieldColumnsMap;
		const size_t COL_MAX = nearbyTickets[0].size();
		for (const FieldData& fieldData : fieldDataArray)
		{
			for (size_t i = 0; i < COL_MAX; ++i)
			{
				bool bIsValidColumn = true;
				for (const auto& nearbyTicket : nearbyTickets)
				{
					int ticketNumber = nearbyTicket[i];
					bIsValidColumn &= fieldData.IsValidTicketNumber(ticketNumber);
				}
				if (bIsValidColumn)
				{
					possibleFieldColumnsMap[fieldData.fieldName].emplace(i);
				}
			}
		}

		// Iterate through each column with the lowest number of options (should be 1...) and assign that option to OrderedColumns
		vector<string> orderedColumns(possibleFieldColumnsMap.size());
		while (possibleFieldColumnsMap.size() > 0)
		{
			string minFieldName = "";
			for (const auto& possibleFieldColumnsPair : possibleFieldColumnsMap)
			{
				if (minFieldName.size() == 0 || possibleFieldColumnsPair.second.size() < possibleFieldColumnsMap[minFieldName].size())
				{
					minFieldName = possibleFieldColumnsPair.first;
				}
			}

			// Assign the field to the ordered column, remove that field from the map
			size_t colIdx = *possibleFieldColumnsMap[minFieldName].begin();
			orderedColumns[colIdx] = minFieldName;
			possibleFieldColumnsMap.erase(minFieldName);

			// Remove the ordered column as an option from the rest of the map
			for (auto& possibleFieldColumnsPair : possibleFieldColumnsMap)
			{
				possibleFieldColumnsPair.second.erase(colIdx);
			}
		}

		// On our ticket, multiply each column with "departure" in it, using our new ordered column array
		long long multipliedDepartures = 1;
		for (size_t i = 0; i < orderedColumns.size(); ++i)
		{
			if (orderedColumns[i].substr(0, 9) == "departure")
			{
				multipliedDepartures *= static_cast<long long>(myTicket[i]);
			}
		}

		os << "Multiplied Departures: " << multipliedDepartures << endl;
	}
}