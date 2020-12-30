#include "Days.h"
#include <map>
#include <set>
#include <vector>

struct HexPoint
{
    HexPoint() :
        x(0), y(0), z(0)
    {}
    HexPoint(int inX, int inY, int inZ) :
        x(inX), y(inY), z(inZ)
    {}

    bool operator==(const HexPoint& p) const
    {
        return(x == p.x && y == p.y && z == p.z);
    }
    bool operator!=(const HexPoint& p) const
    {
        return(x != p.x || y != p.y || z != p.z);
    }
    bool operator<(const HexPoint& p) const
    {
        return(x < p.x || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z));
    }

	HexPoint MovePointForNav(string navInstruction) const
	{
		// Create copy of this point
		HexPoint point = *this;
		if (navInstruction == "e")
		{
			++point.x;
			++point.z;
		}
		else if (navInstruction == "w")
		{
			--point.x;
			--point.z;
		}
		else if (navInstruction == "ne")
		{
			--point.y;
			++point.z;
		}
		else if (navInstruction == "sw")
		{
			++point.y;
			--point.z;
		}
		else if (navInstruction == "nw")
		{
			--point.x;
			--point.y;
		}
		else if (navInstruction == "se")
		{
			++point.x;
			++point.y;
		}
		
		// Return the modified copy
		return point;
	}

    int x;
    int y;
	int z;
};

int GetBlackTileCount(const map<HexPoint, char>& hexGrid)
{
	int blackTileCount = 0;
	for (const auto& it : hexGrid)
	{
		blackTileCount += static_cast<int>(it.second == 'B');
	}
	return blackTileCount;
}

char GetTileColor(const map<HexPoint, char>& hexGrid, const HexPoint& hexPoint)
{
	// If the point is already known on the Grid, grab its color, else the color must be white!
	auto it = hexGrid.find(hexPoint);
	char tileColor = (it == hexGrid.end() ? 'W' : (*it).second);
	return tileColor;
}

template <>
void Run<Day24>(Part part, istream& is, ostream& os)
{
	// Mapping of input line index to a list of instructions
	vector<vector<string>> instructionsMap = {};
	
	string arg;
	while (getline(is, arg))
	{
		instructionsMap.push_back({});
		vector<string>& instructions = instructionsMap.back();
		for (size_t i = 0; i < arg.length();)
		{
			size_t instructionLen = ((arg[i] == 'e' || arg[i] == 'w') ? 1 : 2);
			instructions.push_back(arg.substr(i, instructionLen));
			i += instructionLen;
		}
	}

	// Mapping of a point to it's current color (B or W)
	map<HexPoint, char> hexGrid;

	for (const vector<string>& instructions : instructionsMap)
	{
		// Navigate a relative point from 0,0,0 based on the navigation instructions
		HexPoint point;
		for (const string& instruction : instructions)
		{
			point = point.MovePointForNav(instruction);
		}

		// Hitting an already known point requires flipping it
		char& color = hexGrid[point];
		if (color == 'B')
		{
			color = 'W';
		}
		else
		{
			color = 'B';
		}
	}

	if(part == Part01)
	{
		os << "Total Black Tiles: " << GetBlackTileCount(hexGrid) << endl;
	}
	else if(part == Part02)
	{
		const vector<string> adjacentNavs = {"e", "w", "ne", "nw", "se", "sw"};
		const size_t DAYS_MAX = 100;
		for (size_t i = 1 ; i <= DAYS_MAX; ++i)
		{
			// Construct a set of points that need to have their adjacent points checked
			set<HexPoint> consideredPoints;
			for (const auto& it : hexGrid)
			{
				const HexPoint& referencePoint = it.first;
				const char& referenceColor = it.second;

				// Always consider reference points, these are already known
				consideredPoints.emplace(referencePoint);

				// If the point is black, also consider all points around it (some of these points may be outside the currently known hexGrid)
				if (referenceColor == 'B')
				{
					for (const string& nav : adjacentNavs)
					{
						HexPoint adjacentPoint = referencePoint.MovePointForNav(nav);
						consideredPoints.emplace(adjacentPoint);
					}
				}
			}
			
			// Copy the hex grid, so tiles can be updated simultaneously
			map<HexPoint, char> hexGridCopy = hexGrid;
			for (const HexPoint& consideredPoint : consideredPoints)
			{
				int blackCount = 0;
				for (const string& nav : adjacentNavs)
				{
					// We only need to know how many black tiles are adjacent to the considered point
					HexPoint adjacentPoint = consideredPoint.MovePointForNav(nav);
					char adjacentColor = GetTileColor(hexGrid, adjacentPoint);
					blackCount += static_cast<int>(adjacentColor == 'B');
				}
				
				const char& consideredColor = GetTileColor(hexGrid, consideredPoint);
				if (consideredColor == 'B' && (blackCount == 0 || blackCount > 2))
				{
					// Black tile flips to white!
					hexGridCopy[consideredPoint] = 'W';
				}
				else if (consideredColor == 'W' && blackCount == 2)
				{
					// White tile flips to black!
					hexGridCopy[consideredPoint] = 'B';
				}
			}

			// Assign the fully updated grid
			hexGrid = hexGridCopy;
			if (i % 10 == 0)
			{
				os << "Day " << i << ": " << GetBlackTileCount(hexGrid) << endl;
			}
		}
	}
}