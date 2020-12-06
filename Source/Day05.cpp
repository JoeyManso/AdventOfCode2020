#include "Days.h"
#include "AdventMath.h"
#include <vector>
#include <algorithm>

template <>
void Run<Day05>(Part part, istream& is, ostream& os)
{
	string arg;
	vector<size_t> occupiedSeats;
	
	os << "Calculating Seats..." << endl << endl;
	while(getline(is, arg))
	{
		size_t rowMin = 0;
		size_t rowMax = 127;
		size_t colMin = 0;
		size_t colMax = 7;
		for (const char c : arg)
		{
			switch (c)
			{
				case 'B':
					rowMin += ((rowMax + 1) - rowMin) / 2;
					break;
				case 'F':
					rowMax -= ((rowMax + 1) - rowMin) / 2;
					break;
				case 'R':
					colMin += ((colMax + 1) - colMin) / 2;
					break;
				case 'L':
					colMax -= ((colMax + 1) - colMin) / 2;
					break;
			}
		}
		size_t seat = ((rowMin * 8) + colMin);
		occupiedSeats.push_back(seat);
	}

	// Sort seats to quickly find the max and missing seats
	sort(occupiedSeats.begin(), occupiedSeats.end());
	if(part == Part01)
	{
		os << "Max Seat: " << *(occupiedSeats.end() - 1) << endl;
	}
	else if(part == Part02)
	{
		// All seats should be occupied, find the missing seat in sequence
		size_t seatId = *(occupiedSeats.begin());
		for (size_t i = 0; i < occupiedSeats.size(); ++i, ++seatId)
		{
			if (seatId != occupiedSeats[i])
			{
				os << "Missing Seat: " << seatId << endl;
				break;
			}
		}
	}
}