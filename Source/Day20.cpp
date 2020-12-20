#include "Days.h"
#include <array>
#include <regex>
#include <vector>

const regex RX_TILE_ID(R"(Tile (\d+):)");

enum ESides
{
	TOP = 0,
	BOTTOM = 1,
	LEFT = 2,
	RIGHT = 3,
	MAX = 4
};

enum ETransformedTile
{
	BASE = 0,
	FLIP = 1,
	ROTATE90 = 2,
	ROTATE180 = 3,
	ROTATE270 = 4,
	ROTATEFLIP90 = 5,
	ROTATEFLIP180 = 6,
	ROTATEFLIP270 = 7,
};

struct ImageTile
{
	ImageTile()
		: TileId(0), ImageDataGrid({}), SideMatches({0,0,0,0})
	{
	}

	ImageTile(long long inTileId)
		: TileId(inTileId), ImageDataGrid({}), SideMatches({0,0,0,0})
	{
	}

	void AddLine(string line)
	{
		for (size_t i = 0; i < line.size(); ++i)
		{
			while (ImageDataGrid.size() <= i)
			{
				ImageDataGrid.push_back({});
			}
			ImageDataGrid[i].push_back(line[i]);
		}
	}

	void FlipX()
	{
		size_t x_max = ImageDataGrid.size();
		size_t y_max = ImageDataGrid[0].size();
		for (size_t y = 0; y < y_max; ++y)
		{
			for (size_t x = 0; x < x_max / 2; ++x)
			{
				size_t swapX = ImageDataGrid.size() - (x + 1);
				char tmp = ImageDataGrid[x][y];
				ImageDataGrid[x][y] = ImageDataGrid[swapX][y];
				ImageDataGrid[swapX][y] = tmp;
			}
		}
	}

	void FlipY()
	{
		for (vector<char>& ImageDataColumn : ImageDataGrid)
		{
			for (size_t y = 0; y < ImageDataColumn.size() / 2; ++y)
			{
				size_t swapIdx = ImageDataColumn.size() - (y + 1);
				char tmp = ImageDataColumn[y];
				ImageDataColumn[y] = ImageDataColumn[swapIdx];
				ImageDataColumn[swapIdx] = tmp;
			}
		}
	}

	void Rotate(int dir)
	{
		size_t x_max = ImageDataGrid.size();
		size_t y_max = ImageDataGrid[0].size();
		
		// Reserve rotated grid size
		vector<vector<char>> ImageDataGridRotated = {};
		while (ImageDataGridRotated.size() < x_max)
		{
			ImageDataGridRotated.push_back({});
			ImageDataGridRotated.back().resize(y_max);
		}

		for (size_t x = 0; x < x_max; ++x)
		{
			for (size_t y = 0; y < y_max; ++y)
			{
				size_t rotatedX = 0, rotatedY = 0;
				switch (dir)
				{
					// Rotate values to the left
					case -1:
						rotatedX = y;
						rotatedY = x_max - (x + 1);
						break;
					// Rotate values to the right
					case 1:
						rotatedX = y_max - (y + 1);
						rotatedY = x;
						break;
				}
				ImageDataGridRotated[rotatedX][rotatedY] = ImageDataGrid[x][y];
			}
		}
		ImageDataGrid = ImageDataGridRotated;
	}

	void RotateLeft()
	{
		Rotate(-1);
	}
	
	void RotateRight()
	{
		Rotate(1);
	}

	void CheckAdjacent(const ImageTile& other)
	{
		size_t x_max = ImageDataGrid.size();
		size_t y_max = ImageDataGrid[0].size();

		bool bAdjacentTop = true, bAdjacentBottom = true, bAdjacentLeft = true, bAdjacentRight = true;
		for (size_t i = 0; i < x_max; ++i)
		{
			bAdjacentTop &= ImageDataGrid[i][0] == other.ImageDataGrid[i][y_max - 1];
			bAdjacentBottom &= ImageDataGrid[i][y_max - 1] == other.ImageDataGrid[i][0];
			bAdjacentLeft &= ImageDataGrid[0][i] == other.ImageDataGrid[x_max - 1][i];
			bAdjacentRight &= ImageDataGrid[x_max - 1][i] == other.ImageDataGrid[0][i];
		}

		SideMatches[ESides::TOP] += static_cast<int>(bAdjacentTop);
		SideMatches[ESides::BOTTOM] += static_cast<int>(bAdjacentBottom);
		SideMatches[ESides::LEFT] += static_cast<int>(bAdjacentLeft);
		SideMatches[ESides::RIGHT] += static_cast<int>(bAdjacentRight);
	}

	friend ostream& operator<<(ostream& os, const ImageTile& t)
	{
		os << "Tile " << t.TileId << ":" << endl;
		
		size_t x_max = t.ImageDataGrid.size();
		size_t y_max = t.ImageDataGrid[0].size();
		for (size_t y = 0; y < y_max; ++y)
		{
			for (size_t x = 0; x < x_max; ++x)
			{
				os << t.ImageDataGrid[x][y];
			}
			os << endl;
		}
		return os;
	}
	
	long long TileId;
	vector<vector<char>> ImageDataGrid;
	array<int, ESides::MAX> SideMatches;
};

template <>
void Run<Day20>(Part part, istream& is, ostream& os)
{
	vector<ImageTile> imageTiles = {};
	
	string arg;
	while (getline(is, arg))
	{
		smatch match;
		if (regex_search(arg, match, RX_TILE_ID))
		{
			imageTiles.push_back(stoll(match.str(1)));
		}
		else
		{
			ImageTile& imageTile = imageTiles.back();
			imageTile.AddLine(arg);
		}
	}
	
// 	for (const ImageTile& imageTile : imageTiles)
// 	{
// 		os << imageTile << endl;
// 	}

	ImageTile& imageTile = imageTiles[0];
	
	os << "Base:" << endl;
	os << imageTile << endl;

// 	os << "FlipX:" << endl;
// 	imageTile.FlipX();
// 	os << imageTile << endl;
// 	imageTile.FlipX();
// 	os << imageTile << endl;
// 
// 	os << "FlipY:" << endl;
// 	imageTile.FlipY();
// 	os << imageTile << endl;
// 	imageTile.FlipY();
// 	os << imageTile << endl;

// 	os << "RotateLeft:" << endl;
// 	imageTile.RotateLeft();
// 	os << imageTile << endl;
// 
// 	os << "RotateRight:" << endl;
// 	imageTile.RotateRight();
// 	os << imageTile << endl;	

// 	os << "RotateRight:" << endl;
// 	imageTile.RotateRight();
// 	os << imageTile << endl;
// 
// 	os << "RotateLeft:" << endl;
// 	imageTile.RotateLeft();
// 	os << imageTile << endl;

	for (size_t i = 0; i < imageTiles.size(); ++i)
	{
		for (size_t j = 0; j < imageTiles.size(); ++j)
		{
			if (i != j)
			{
				imageTiles[i].CheckAdjacent(imageTiles[j]);
			}
		}
	}
	
	if(part == Part01)
	{
		for (const ImageTile& imageTile : imageTiles)
		{
			os << "Tile " << imageTile.TileId << " Adjacent Count:" << endl;
			os << "T: " << imageTile.SideMatches[ESides::TOP] << " B: " << imageTile.SideMatches[ESides::BOTTOM] << " L: " << imageTile.SideMatches[ESides::LEFT] << " R: " << imageTile.SideMatches[ESides::RIGHT] << endl << endl;
		}
	}
	else if(part == Part02)
	{

	}
}