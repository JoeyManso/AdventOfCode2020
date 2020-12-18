#include "Days.h"
#include <map>

const char STATE_ACTIVE = '#';
const char STATE_INACTIVE = '.';

size_t CYCLES_MAX = 6;

struct Point4D
{
	Point4D()
		: x(0), y(0), z(0), w(0)
	{
	}

	Point4D(int inX, int inY, int inZ, int inW)
		: x(inX), y(inY), z(inZ), w(inW)
	{
	}

	bool operator==(const Point4D& p) const
    {
        return(x == p.x && y == p.y && z == p.z && w == p.w);
    }
    bool operator!=(const Point4D& p) const
    {
        return(x != p.x || y != p.y || z != p.z || w != p.w);
    }
	bool operator<(const Point4D& p) const
    {
        return(x < p.x || (x == p.x && y < p.y) || (x == p.x && y == p.y && z < p.z) || (x == p.x && y == p.y && z == p.z && w < p.w));
    }
	
	int x, y, z, w;
};

size_t GetActiveNeighborCount(const Point4D& coordinates, const map<Point4D, char>& coordinateCubeMap)
{
	size_t neighborCount = 0;
	for (int x = coordinates.x - 1; x <= coordinates.x + 1; ++x)
	{
		for (int y = coordinates.y - 1; y <= coordinates.y + 1; ++y)
		{
			for (int z = coordinates.z - 1; z <= coordinates.z + 1; ++z)
			{
				for (int w = coordinates.w - 1; w <= coordinates.w + 1; ++w)
				{
					Point4D searchCoordinates(x, y, z, w);
					if (coordinates != searchCoordinates)
					{
						auto cubeState = coordinateCubeMap.find(searchCoordinates);
						if (cubeState != coordinateCubeMap.end() && (*cubeState).second == STATE_ACTIVE)
						{
							++neighborCount;
						}
					}
				}
			}
		}
	}
	return neighborCount;
}

template <>
void Run<Day17>(Part part, istream& is, ostream& os)
{
	// Mapping of Points to their cube state
	map<Point4D, char> coordinateCubeMap;
	
	// Gather initial coordinates
	string arg;
	int numLines = 0;
	while (getline(is, arg))
	{
		for (int x = 0; x < arg.size(); ++x)
		{
			coordinateCubeMap[Point4D(x, numLines, 0, 0)] = arg[x];
		}
		++numLines;
	}
	
	int x_min = 0, x_max = numLines - 1, y_min = 0, y_max = numLines - 1, z_min = 0, z_max = 0, w_min = 0, w_max = 0;
	for (size_t cycle = 1; cycle <= CYCLES_MAX; ++cycle)
	{		
		// Debug Printing
// 		for (int w = w_min; w <= w_max; ++w)
// 		{
// 			for (int z = z_min; z <= z_max; ++z)
// 			{
// 				os << "z=" << z << ", w=" << w << endl;
// 				for (int y = y_min; y <= y_max; ++y)
// 				{
// 					for (int x = x_min; x <= x_max; ++x)
// 					{
// 						os << coordinateCubeMap[Point4D(x, y, z, w)];
// 					}
// 					os << endl;
// 				}
// 			}
// 			os << endl;
// 		}

		// Expand the map with inactive cubes on each dimension border each cycle
		--x_min;
		--y_min;
		--z_min;
		++x_max;
		++y_max;
		++z_max;
		if (part == Part02)
		{
			// 4D expansion for part 2 only!
			--w_min;
			++w_max;
		}
		for (int x = x_min; x <= x_max; ++x)
		{
			for (int y = y_min; y <= y_max; ++y)
			{
				for (int z = z_min; z <= z_max; ++z)
				{
					for (int w = w_min; w <= w_max; ++w)
					{
						Point4D searchCoordinates(x, y, z, w);
						if (coordinateCubeMap.find(searchCoordinates) == coordinateCubeMap.end())
						{
							coordinateCubeMap[searchCoordinates] = STATE_INACTIVE;
						}
					}
				}
			}
		}

		// Create a copy of the cube map to update each cube state simultaneously
		os << "Processing Cycle " << cycle << "..." << endl;
		map<Point4D, char> coordinateCubeMapCopy = coordinateCubeMap;
		for (const auto& coordinateCubePair : coordinateCubeMap)
		{
			const Point4D& coordinates = coordinateCubePair.first;
			const char cubeState = coordinateCubePair.second;
			size_t activeNeighbors = GetActiveNeighborCount(coordinates, coordinateCubeMap);
			switch (cubeState)
			{
				case STATE_ACTIVE:
					if (activeNeighbors != 2 && activeNeighbors != 3)
					{
						coordinateCubeMapCopy[coordinates] = STATE_INACTIVE;
					}
					break;
				case STATE_INACTIVE:
					if (activeNeighbors == 3)
					{
						coordinateCubeMapCopy[coordinates] = STATE_ACTIVE;
					}
					break;					
			}
		}
		coordinateCubeMap = coordinateCubeMapCopy;
	}

	size_t numActive = 0;
	for (const auto& coordinateCubeMapPair : coordinateCubeMap)
	{
		if (coordinateCubeMapPair.second == STATE_ACTIVE)
		{
			++numActive;
		}
	}
	os << "Remaining Active Cubes: " << numActive << endl;
}