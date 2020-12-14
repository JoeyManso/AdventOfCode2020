#include "Days.h"
#include "Point2D.h"
#include "Vector2D.h"
#include <regex>
#include <vector>

const regex RX_NAV(R"(([A-Z])(\d+))");

Vector2D GetDirectionVector(char direction)
{
	Vector2D vecDirection;
	switch (direction)
	{
		case 'N':
			vecDirection = Vector2D(0, 1);
			break;
		case 'S':
			vecDirection = Vector2D(0, -1);
			break;
		case 'E':
			vecDirection = Vector2D(1, 0);
			break;
		case 'W':
			vecDirection = Vector2D(-1, 0);
			break;
	}
	return vecDirection;
}

struct Ship
{
	Ship(const Point2D& inShipPosition, const Point2D& inWaypointPosition)
		: ShipPosition(inShipPosition), WaypointRelativePosition(inWaypointPosition)
	{
		Direction = WaypointRelativePosition - ShipPosition;
		Direction.Normalize();
	}

	void Move(Part part, char command, int amount)
	{
		Vector2D shipMovement = Vector2D(), waypointMovement = Vector2D();
		if (part == Part01)
		{
			switch (command)
			{
				case 'F':
					shipMovement = Direction * amount;
					break;
				case 'R':
					amount *= -1;
				case 'L':
					Direction.RotateByDegrees(static_cast<double>(amount));
					break;
				case 'N':
				case 'S':
				case 'E':
				case 'W':
					shipMovement = GetDirectionVector(command) * amount;
					break;
			}
		}
		else if (part == Part02)
		{
			switch (command)
			{
				case 'F':
					shipMovement = Vector2D(WaypointRelativePosition - Point2D()) * amount;
					break;
				case 'R':
					amount *= -1;
				case 'L':
					{
						Vector2D WaypointDistance = WaypointRelativePosition - Point2D();
						WaypointDistance.RotateByDegrees(static_cast<double>(amount));
						WaypointRelativePosition = Point2D() + WaypointDistance;
					}
					break;
				case 'N':
				case 'S':
				case 'E':
				case 'W':
					waypointMovement = GetDirectionVector(command) * amount;
					break;
			}
		}
		ShipPosition += shipMovement;
		WaypointRelativePosition += waypointMovement;
	}

	friend ostream& operator<<(ostream& os, const Ship& s)
	{
		os << "Ship Position: (" << s.ShipPosition.x << ", " << s.ShipPosition.y << ")" << endl;
		os << "Waypoint Relative Position: (" << s.WaypointRelativePosition.x << ", " << s.WaypointRelativePosition.y << ")" << endl;
		os << "Ship Direction: (" << s.Direction.x << ", " << s.Direction.y << ")" << endl;
		return os;
	}

	Point2D ShipPosition;
	Point2D WaypointRelativePosition;
	Vector2D Direction;
};

template <>
void Run<Day12>(Part part, istream& is, ostream& os)
{
	Ship ship = Ship(Point2D(), Point2D(10, 1));
	os << "Starting Position" << endl;
	os << ship << endl;
	
	string arg;
	while (getline(is, arg))
	{
		smatch match;
		regex_search(arg, match, RX_NAV);
		ship.Move(part, match.str(1)[0], stoi(match.str(2)));
		//os << endl << "Moving " << match.str(1) << " - " << match.str(2) << endl;
		//os << ship << endl;
	}
	int manhattanDist = abs(ship.ShipPosition.x) + abs(ship.ShipPosition.y);
	os << "Manhattan Distance: " << manhattanDist << endl;
}