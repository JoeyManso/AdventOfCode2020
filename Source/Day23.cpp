#include "Days.h"
#include "AdventUtils.h"
#include <list>

size_t MAX_MOVES = 100;

template <>
void Run<Day23>(Part part, istream& is, ostream& os)
{
	int labelMax = 0;
	list<int> crabCups;
	string arg;
	while (getline(is, arg))
	{
		for (char c : arg)
		{
			int cupLabel = CharToInt(c);
			labelMax = max(cupLabel, labelMax);
			crabCups.push_back(cupLabel);
		}
	}

	list<int>::iterator itCurrent = crabCups.begin();
	
	for (size_t i = 1; i <= MAX_MOVES; ++i)
	{
		os << "-- Move " << i << " --" << endl;
		os << "cups: ";
		for (list<int>::iterator it = crabCups.begin(); it != crabCups.end(); ++it)
		{
			if (it == itCurrent)
			{
				os << "(" << *it << ")";
			}
			else
			{
				os << " " << *it << " ";
			}
		}
		os << endl;
		
		// The crab picks up the three cups that are immediately clockwise of the current cup.
		// They are removed from the circle; 
		// cup spacing is adjusted as necessary to maintain the circle.
		list<int> removedCups = {};
		list<int>::iterator itRemove = next(itCurrent);
		while (removedCups.size() < 3)
		{
			if (itRemove == crabCups.end())
			{
				itRemove = crabCups.begin();
			}
			removedCups.push_back(*itRemove);
			itRemove = crabCups.erase(itRemove);
		}

		os << "pick up: ";
		for (list<int>::iterator it = removedCups.begin(); it != removedCups.end(); ++it)
		{
			if (it != removedCups.begin())
			{
				os << ", ";
			}
			os << *it;
		}
		os << endl;

		// The crab selects a destination cup: the cup with a label equal to the current cup's label minus one.
		// If this would select one of the cups that was just picked up, 
		// the crab will keep subtracting one until it finds a cup that wasn't just picked up. 
		// If at any point in this process the value goes below the lowest value on any cup's label, 
		// it wraps around to the highest value on any cup's label instead.
		int labelDestiniation = *itCurrent;
		list<int>::iterator itDestiniation = crabCups.end();
		while (itDestiniation == crabCups.end())
		{
			--labelDestiniation;
			if (labelDestiniation <= 0)
			{
				labelDestiniation = labelMax;
			}
			for (list<int>::iterator it = crabCups.begin(); it != crabCups.end(); ++it)
			{
				if (*it == labelDestiniation)
				{
					itDestiniation = it;
					break;
				}
			}
		}
		os << "destination: " << (*itDestiniation) << endl << endl;

		// The crab places the cups it just picked up so that they are immediately clockwise of the destination cup.
		// They keep the same order as when they were picked up.
		crabCups.insert(next(itDestiniation), removedCups.begin(), removedCups.end());

		// The crab selects a new current cup: the cup which is immediately clockwise of the current cup
		itCurrent = next(itCurrent);
		if (itCurrent == crabCups.end())
		{
			itCurrent = crabCups.begin();
		}
	}


	// After the crab is done, what order will the cups be in? 
	// Starting after the cup labeled 1, collect the other cups' labels clockwise into a single string with no extra characters; 
	// each number except 1 should appear exactly once.
	list<int>::iterator itOne = crabCups.end();
	while (itOne == crabCups.end())
	{
		for (list<int>::iterator it = crabCups.begin(); it != crabCups.end(); ++it)
		{
			if (*it == 1)
			{
				itOne = it;
				break;
			}
		}
	}

	os << "Final Cup Sequence: ";
	for (list<int>::iterator it = next(itOne); it != itOne;)
	{
		if (it == crabCups.end())
		{
			it = crabCups.begin();
		}
		os << *it;
		it = next(it);
	}

	if(part == Part01)
	{

	}
	else if(part == Part02)
	{

	}
}