#include "Days.h"
#include "AdventMath.h"
#include "AdventUtils.h"
#include <set>
#include <map>
#include <vector>

size_t MAX_CUPS_P2 = 1000000;
size_t MAX_MOVES_P1 = 100;
size_t MAX_MOVES_P2 = 10000000;

struct Cup
{
	Cup(int inLabel)
		: label(inLabel)
	{
	}

	friend ostream& operator<<(ostream& os, const Cup* c)
	{
		os << c->label;
		return os;
	}

	int label;
	Cup* next;
};

template <>
void Run<Day23>(Part part, istream& is, ostream& os)
{
	// Map of a Cup's label to that Cup
	map<int, Cup*> cupMap;
	int labelMax = 0;
	int labelFirst = 0;
	
	// Collect cup data
	{
		int labelPrev = 0;
		string arg;
		while (getline(is, arg))
		{
			for (char c : arg)
			{
				int cupLabel = CharToInt(c);
				cupMap[cupLabel] = new Cup(cupLabel);
				if (labelPrev)
				{
					cupMap[labelPrev]->next = cupMap[cupLabel];
				}
				else
				{
					labelFirst = cupLabel;
				}
				labelPrev = cupLabel;
			}
		}

		if (part == Part02)
		{
			for (int cupLabel = static_cast<int>(cupMap.size()) + 1; cupLabel <= MAX_CUPS_P2; ++cupLabel)
			{
				cupMap[cupLabel] = new Cup(cupLabel);
				cupMap[labelPrev]->next = cupMap[cupLabel];
				labelPrev = cupLabel;
			}
		}
		
		// Point the last cup to the first cup to complete the circle
		cupMap[labelPrev]->next = cupMap[labelFirst];

		// Assign the max possible label based on # cups
		labelMax = static_cast<int>(cupMap.size());
	}
	
	const size_t maxMoves = part == Part01 ? MAX_MOVES_P1 : MAX_MOVES_P2;
	const float maxMovesFlt = static_cast<float>(maxMoves);
	set<int> completionMilestones = {};
	Cup* cupCurrent = cupMap[labelFirst];
	for (size_t i = 1; i <= maxMoves; ++i)
	{
		int completionPct = static_cast<int>(round((static_cast<float>(i) / maxMovesFlt) * 100.0f));
		if (completionMilestones.find(completionPct) == completionMilestones.end() && completionPct > 0 && completionPct % 5 == 0)
		{
			os << completionPct << "% Complete..." << endl;
			completionMilestones.emplace(completionPct);
		}
// 		os << "-- Move " << i << " --" << endl;
// 		os << "cups: (" << cupCurrent << ")";
// 		for (const Cup* cup = cupCurrent->next; cup != cupCurrent; cup = cup->next)
// 		{
// 			os << " " << cup << " ";
// 		}
// 		os << endl;
		
		// The crab picks up the three cups that are immediately clockwise of the current cup.
		// They are removed from the circle; 
		// cup spacing is adjusted as necessary to maintain the circle.
		vector<Cup*> removedCups;
		for (Cup* cup = cupCurrent->next; removedCups.size() < 3; cup = cup->next)
		{
			removedCups.push_back(cup);
		}
		// Re-wire current cup to point to the next cup of the last removed cup (effectively removing all cups from the circle)
		cupCurrent->next = removedCups.back()->next;

// 		os << "pick up: ";
// 		for (const Cup* removedCup : removedCups)
// 		{
// 			if (removedCup != *removedCups.begin())
// 			{
// 				os << ", ";
// 			}
// 			os << removedCup;
// 		}
// 		os << endl;

		// The crab selects a destination cup: the cup with a label equal to the current cup's label minus one.
		// If this would select one of the cups that was just picked up, 
		// the crab will keep subtracting one until it finds a cup that wasn't just picked up. 
		// If at any point in this process the value goes below the lowest value on any cup's label, 
		// it wraps around to the highest value on any cup's label instead.
		
		// Adjust destination label if the destination cup was removed 
		int labelDestiniation = cupCurrent->label - 1;
		bool bStillSearching = true;
		while (bStillSearching)
		{
			bStillSearching = false;
			if (labelDestiniation <= 0)
			{
				labelDestiniation = labelMax;
			}
			for (const Cup* removedCup : removedCups)
			{
				if (removedCup->label == labelDestiniation)
				{
					--labelDestiniation;
					bStillSearching = true;
				}
			}
		}
		Cup* cupDestiniation = cupMap[labelDestiniation];
//		os << "destination: " << cupDestiniation << endl << endl;

		// The crab places the cups it just picked up so that they are immediately clockwise of the destination cup.
		// They keep the same order as when they were picked up.
		
		// Re-wire cups so the destination cup inserts the removed cups
		Cup* tmp = cupDestiniation->next;
		cupDestiniation->next = removedCups.front();
		removedCups.back()->next = tmp;

		// The crab selects a new current cup: the cup which is immediately clockwise of the current cup
		cupCurrent = cupCurrent->next;
	}

	// After the crab is done, what order will the cups be in? 
	// Starting after the cup labeled 1, collect the other cups' labels clockwise into a single string with no extra characters; 
	// each number except 1 should appear exactly once.
	Cup* cupOne = cupMap[1];

	if(part == Part01)
	{
		os << endl << "Final Cup Sequence: ";
		for (const Cup* cup = cupOne->next; cup != cupOne; cup = cup->next)
		{
			os << cup;
		}
		os << endl;
	}
	else if(part == Part02)
	{
		os << endl;
		os << "Star Cup 1: " << (cupOne->next) << endl;
		os << "Star Cup 2: " << (cupOne->next->next) << endl;
		os << "Multiplied Cups: " << static_cast<long long>(cupOne->next->label) * static_cast<long long>(cupOne->next->next->label) << endl;
	}
}