#include "Days.h"
#include "Point2D.h"
#include "Vector2D.h"
#include <vector>

using namespace std;

const char TREE_CHAR = '#';

template <>
void Run<Day03>(Part part, istream& is, ostream& os)
{
	os << "Parsing Terrain..." << endl;
	string arg;
	vector<string> terrainLines;
	while(getline(is, arg))
	{
		terrainLines.push_back(arg);
	}

	// Add required Terrain Slopes for each part
	vector<Vector2D> terrainSlopes;
	if(part == Part01)
	{
		terrainSlopes.push_back(Vector2D(3, 1));
	}
	else if(part == Part02)
	{
		terrainSlopes.push_back(Vector2D(1, 1));
		terrainSlopes.push_back(Vector2D(3, 1));
		terrainSlopes.push_back(Vector2D(5, 1));
		terrainSlopes.push_back(Vector2D(7, 1));
		terrainSlopes.push_back(Vector2D(1, 2));
	}
	
	// Iterate through the terrain coordinates with each slope, tracking the total trees found and running product of that count
	size_t treeCountsProduct = 1;
	for (const Vector2D terrainSlope : terrainSlopes)
	{
		size_t treeCount = 0;
		Point2D currentCoordinates = Point2D();
		while (true)
		{
			currentCoordinates += terrainSlope;
			if (currentCoordinates.y < terrainLines.size())
			{
				const string terrainLine = terrainLines[currentCoordinates.y];
				
				// Because the terrain pattern repeats horizontally, normalize the x coordinate by wrapping around when necessary
				size_t normalizedX = currentCoordinates.x;
				while (normalizedX >= terrainLine.length())
				{
					normalizedX -= terrainLine.length();
				}
				if (terrainLine[normalizedX] == TREE_CHAR)
				{
					++treeCount;
				}
			}
			else
			{
				break;
			}
		}
		treeCountsProduct *= treeCount;
		os << "Navigating Trees with Slope (" << terrainSlope.x << ", " << terrainSlope.y << ")" << endl;
		os << "    Tree Count " << treeCount << endl;
	}
	os << endl << "Tree Count Product of all slopes: " << treeCountsProduct << endl;
}