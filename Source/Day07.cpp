#include "Days.h"
#include <map>
#include <regex>

const regex RX_BAG_CONTAINER = regex(R"((.+) bags contain)");
const regex RX_BAG_RULE = regex(R"((\d+) ([a-z\s]+) bag)");
const string TEST_BAG_TYPE = "shiny gold";

/** Mapping of bag type to its rules */
map<string, struct BagRules*> BagRuleMap = {};

struct BagRules
{
	BagRules()
		: ChildBagCounts({})
	{
	}

	void AddChildBagRule(string bagType, size_t bagAmount)
	{
		ChildBagCounts.emplace(bagType, bagAmount);
	}

	/** Returns true if this bag or any of its child-bags can contain the given bag type */
	bool CanContain(string bagType) const
	{
		for (const auto& childBagPair : ChildBagCounts)
		{
			string childBagType = childBagPair.first;
			if (childBagType == bagType || BagRuleMap[childBagType]->CanContain(bagType))
			{
				return true;
			}
		}
		return false;
	}

	/** Returns the maximum numbers of bags this bag can contain */
	size_t GetContainCount() const
	{
		size_t containCount = 0;
		for (const auto childBagPair : ChildBagCounts)
		{
			string childBagType = childBagPair.first;
			size_t childBagCount = childBagPair.second;
			containCount += childBagCount + (childBagCount * BagRuleMap[childBagType]->GetContainCount());
		}
		return containCount;
	}

private:
	/** Mapping for amounts of specific bag types this bag can hold */
	map<string, size_t> ChildBagCounts;
};

template <>
void Run<Day07>(Part part, istream& is, ostream& os)
{
	os << "Creating Bag Rules..." << endl;

	string arg;
	while (getline(is, arg))
	{
		// Parse the parent bag container
		smatch matchBagContainer;
		regex_search(arg, matchBagContainer, RX_BAG_CONTAINER);
		string bagType = matchBagContainer.str(1);

		BagRules* bagRule = new BagRules();
		BagRuleMap.emplace(bagType, bagRule);

		// Loop through to parse the child bag rules
		smatch matchBagRule;
		while(regex_search(arg, matchBagRule, RX_BAG_RULE))
		{
			bagRule->AddChildBagRule(matchBagRule.str(2), stoi(matchBagRule.str(1)));
			arg = matchBagRule.suffix();
		}
	}
	
	size_t ContainCount = 0;
	if(part == Part01)
	{
		os << "Calculating number of bags that can contain " << TEST_BAG_TYPE << " bag..." << endl;
		for (const auto& BagRulePair : BagRuleMap)
		{
			if (BagRulePair.second->CanContain(TEST_BAG_TYPE))
			{
				++ContainCount;
			}
		}
		os << "Contain Count: " << ContainCount << endl;
	}
	else if(part == Part02)
	{
		os << "Calculating total contain count for " << TEST_BAG_TYPE << " bag..." << endl;
		ContainCount = BagRuleMap[TEST_BAG_TYPE]->GetContainCount();
		os << "Contain Count: " << ContainCount << endl;
	}

	// Dealloc master rules
	for (auto& BagRulePair : BagRuleMap)
	{
		delete BagRulePair.second;
		BagRulePair.second = nullptr;
	}
	BagRuleMap.clear();
}