#include "Days.h"
#include <regex>
#include <map>
#include <set>
#include <vector>

const regex RX_ALLERGEN(R"((.+) \(contains (.+)\))");

struct FoodData
{	
	/** Set of all ingredients in this food */
	set<string> Ingredients;

	/** Set of known allergens in this food */
	set<string> Allergens;
};

struct AllergenData
{
	AllergenData()
		: PossibleIngredientTriggers({})
	{
	}

	AllergenData(const set<string>& inPossibleIngredientTriggers)
		: PossibleIngredientTriggers(inPossibleIngredientTriggers)
	{
	}

	void IntersectIngredients(const set<string>& ingredients)
	{
		set<string> intersectingIngredients;
		set_intersection(PossibleIngredientTriggers.begin(), PossibleIngredientTriggers.end(), 
			ingredients.begin(), ingredients.end(),
			inserter(intersectingIngredients, intersectingIngredients.begin()));
		PossibleIngredientTriggers = intersectingIngredients;
	}
	
	/** Set of potential triggers for an allergen */
	set<string> PossibleIngredientTriggers;
};

template <>
void Run<Day21>(Part part, istream& is, ostream& os)
{
	/** Read in all known food data to an array */
	vector<FoodData> foodDataArray;
	
	string arg;
	while (getline(is, arg))
	{
		smatch match;
		if (regex_search(arg, match, RX_ALLERGEN))
		{
			vector<string> ingredients = {""};
			vector<string> allergens = {""};
			for (char c : match.str(1))
			{
				if (c == ' ')
				{
					ingredients.push_back("");
				}
				else
				{
					ingredients.back().push_back(c);
				}
			}
			for (char c : match.str(2))
			{
				if (c == ',')
				{
					allergens.push_back("");
				}
				else if (c != ' ')
				{
					allergens.back().push_back(c);
				}
			}

			foodDataArray.push_back(FoodData());
			foodDataArray.back().Ingredients.insert(ingredients.begin(), ingredients.end());
			foodDataArray.back().Allergens.insert(allergens.begin(), allergens.end());
		}
	}

	/** Mapping of allergen name to currently known data */
	map<string, AllergenData> allergenDataMap;
	for (const FoodData& foodData : foodDataArray)
	{
		for (const string& allergen : foodData.Allergens)
		{
			if (allergenDataMap.find(allergen) != allergenDataMap.end())
			{
				// If an entry for this allergen is found, intersect the currently known triggers with the new data
				allergenDataMap[allergen].IntersectIngredients(foodData.Ingredients);
			}
			else
			{
				// If not found, just add all ingredients listed for the food
				allergenDataMap[allergen] = foodData.Ingredients;
			}
		}
	}

	if(part == Part01)
	{
		// Put all ingredient triggers into a master list for easy search
		set<string> allPossibleIngredientTriggers;
		for (const auto& allergenDataPair : allergenDataMap)
		{
			const set<string>& possibleIngredientTriggers = allergenDataPair.second.PossibleIngredientTriggers;
			allPossibleIngredientTriggers.insert(possibleIngredientTriggers.begin(), possibleIngredientTriggers.end());
		}

		int totalCount = 0;
		for (const FoodData& foodData : foodDataArray)
		{
			for (const string& ingredient : foodData.Ingredients)
			{
				if (allPossibleIngredientTriggers.find(ingredient) == allPossibleIngredientTriggers.end())
				{
					++totalCount;
				}
			}
		}
		os << "Total Non-Allergen ingredients: " << totalCount << endl;
	}
	else if(part == Part02)
	{
		map<string, string> allergenIngredientMap;

		// Iterate through allergen map, find those with one ingredient and assign to the end map
		// Remove that ingredient as a possibility from remaining allergens
		auto allergenDataIt = allergenDataMap.end();
		while (!allergenDataMap.empty())
		{
			if (allergenDataIt == allergenDataMap.end())
			{
				allergenDataIt = allergenDataMap.begin();
			}
			
			const string& allergen = (*allergenDataIt).first;
			const AllergenData& allergenData = (*allergenDataIt).second;
			if (allergenData.PossibleIngredientTriggers.size() == 1)
			{
				// Add the allergen ingredient pair to the known map
				string allergenIngredient = *allergenData.PossibleIngredientTriggers.begin();
				allergenIngredientMap.emplace(allergen, allergenIngredient);

				// Remove the allergen from the data map now that it's been processed
				allergenDataIt = allergenDataMap.erase(allergenDataIt);
					
				// Remove this ingredient from all other allergen data
				for (auto& allergenDataPair : allergenDataMap)
				{
					allergenDataPair.second.PossibleIngredientTriggers.erase(allergenIngredient);
				}
			}
			else
			{
				++allergenDataIt;
			}
		}

		// Print the naughty ingredients
		string allergenIngredientsStr;
		for (const auto& allergenIngredientPair : allergenIngredientMap)
		{
			allergenIngredientsStr += allergenIngredientPair.second + ",";
		}
		allergenIngredientsStr.erase(allergenIngredientsStr.size() - 1, 1);
		os << "Allergen Ingredients: " << allergenIngredientsStr << endl;
	}
}