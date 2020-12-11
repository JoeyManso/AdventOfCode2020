#include "Days.h"
#include "AdventMath.h"
#include <vector>

struct SeatingGrid
{
	static const char EMPTY = 'L';
	static const char OCCUPIED = '#';
	static const char FLOOR = '.';
	const int MAX_ADJ_OCCUPIED[2] = {4, 5};

	SeatingGrid(Part inPuzzlePart)
		: PuzzlePart(inPuzzlePart)
		, CharGrid({})
	{
	}

	/** Input parsing to help construct grid */
	void AddRow(const string& rowStr)
	{
		CharGrid.push_back({});
		for (const char c : rowStr)
		{
			(*(CharGrid.end() - 1)).push_back(c);
		}
	}

	/** Returns the seat type at the given indices, else defaults to neutral FLOOR */
	char GetSeatType(int i, int j) const
	{
		if (InRangeExclusive(i, 0, static_cast<int>(CharGrid.size()))
			&& InRangeExclusive(j, 0, static_cast<int>(CharGrid[i].size())))
		{
			return CharGrid[i][j];
		}
		return FLOOR;
	}

	/** Returns the total count of the given seat type */
	int GetTotalCount(const char& seatType) const
	{
		int totalCount = 0;
		for (const vector<char>& charRow : CharGrid)
		{
			for (char charSeat : charRow)
			{
				if (charSeat == seatType)
				{
					++totalCount;
				}
			}
		}
		return totalCount;
	}

	/** Returns the total adjacent seats from the given indices, depending on PuzzlePart dictated rules */
	int GetAdjacentOccupiedCount(int iGrid, int jGrid) const
	{		
		// Part 1: Search only surrounding adjacent seats
		// Part 2: Search until any occupied/empty seat is found
		const int maxSearchDist = PuzzlePart == Part01 ? 1 : static_cast<int>(CharGrid.size()) - 1;
		char foundSeats[8] = {FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR, FLOOR};
		for (int x = 1; x <= maxSearchDist; ++x)
		{
			// left
			if (foundSeats[0] == FLOOR)
			{
				foundSeats[0] = GetSeatType(iGrid - x, jGrid);
			}
			// right
			if (foundSeats[1] == FLOOR)
			{
				foundSeats[1] = GetSeatType(iGrid + x, jGrid);
			}
			// up
			if (foundSeats[2] == FLOOR )
			{
				foundSeats[2] = GetSeatType(iGrid, jGrid - x);
			}
			// down
			if (foundSeats[3] == FLOOR )
			{
				foundSeats[3] = GetSeatType(iGrid, jGrid + x);
			}
			// left - up
			if (foundSeats[4] == FLOOR )
			{
				foundSeats[4] = GetSeatType(iGrid - x, jGrid - x);
			}
			// right - up
			if (foundSeats[5] == FLOOR )
			{
				foundSeats[5] = GetSeatType(iGrid + x, jGrid - x);
			}
			// left - down
			if (foundSeats[6] == FLOOR)
			{
				foundSeats[6] =  GetSeatType(iGrid - x, jGrid + x);
			}
			// right - down
			if (foundSeats[7] == FLOOR)
			{
				foundSeats[7] = GetSeatType(iGrid + x, jGrid + x);
			}
		}

		// Return the total occupied count
		int occupiedAdjacent = 0;
		for (char foundSeat : foundSeats)
		{
			if (foundSeat == OCCUPIED)
				++occupiedAdjacent;
		}
		return occupiedAdjacent;
	}

	bool RunRules()
	{		
		// Grid needs to update all at once, so make a buffer while reading from the current grid, also helps us to compare at the end
		vector<vector<char>> NewCharGrid = CharGrid;
		for (int i = 0; i < CharGrid.size(); ++i)
		{
			vector<char>& charRow = CharGrid[i];
			for (int j = 0; j < charRow.size(); ++j)
			{
				switch (charRow[j])
				{
					case EMPTY:
						// Occupy this seat if no adjacent seats are occupied
						if (GetAdjacentOccupiedCount(i, j) == 0)
						{
							NewCharGrid[i][j] = OCCUPIED;
						}
						break;
					case OCCUPIED:
						// Leave this seat if too many adjacent seats are occupied
						if (GetAdjacentOccupiedCount(i, j) >= MAX_ADJ_OCCUPIED[PuzzlePart])
						{
							NewCharGrid[i][j] = EMPTY;
						}
						break;
					default:
						break;
				}
			}
		}
		bool bDidChange = CharGrid != NewCharGrid;
		CharGrid = NewCharGrid;
		return bDidChange;
	}

private:
	vector<vector<char>> CharGrid;
	Part PuzzlePart;
};

template <>
void Run<Day11>(Part part, istream& is, ostream& os)
{
	// Construct the initial seating grid
	SeatingGrid seatingGrid = SeatingGrid(part);
	string arg;
	while (getline(is, arg))
	{
		seatingGrid.AddRow(arg);
	}

	// Run the rules until no seats change
	size_t runCount = 0;
	while (seatingGrid.RunRules())
	{
		++runCount;
	}
	os << "Seating grid stabilized after running rules " << runCount << " times " << endl;
	os << "Total occupied seats: " << seatingGrid.GetTotalCount(SeatingGrid::OCCUPIED) << endl;
}