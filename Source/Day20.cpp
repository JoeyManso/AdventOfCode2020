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
};

struct ImageTile
{
	ImageTile(int inTileId)
		: ImageTile(inTileId, {})
	{
	}

	ImageTile(int inTileId, const vector<vector<char>>& InImageDataGrid)
		: TileId(inTileId), ImageDataGrid(InImageDataGrid), NeighborTileIds({0,0,0,0})
	{
	}

	ImageTile Transpose() const
	{
		ImageTile TransposedImage = ImageTile(*this);
		for (size_t x = 0; x < GetTileSize(); ++x)
		{
			for (size_t y = 0; y < GetTileSize(); ++y)
			{
				TransposedImage.ImageDataGrid[x][y] = ImageDataGrid[y][x];
			}
		}
		return TransposedImage;
	}

	ImageTile FlipX() const
	{
		ImageTile FlippedImage = ImageTile(*this);
		for (size_t x = 0; x < GetTileSize() / 2; ++x)
		{
			for (size_t y = 0; y < GetTileSize(); ++y)
			{
				size_t swapX = GetTileSize() - (x + 1);
				char tmp = FlippedImage.ImageDataGrid[x][y];
				FlippedImage.ImageDataGrid[x][y] = FlippedImage.ImageDataGrid[swapX][y];
				FlippedImage.ImageDataGrid[swapX][y] = tmp;
			}
		}
		return FlippedImage;
	}

	ImageTile FlipY() const
	{
		ImageTile FlippedImage = ImageTile(*this);
		for (vector<char>& ImageDataColumn : FlippedImage.ImageDataGrid)
		{
			for (size_t y = 0; y < GetTileSize() / 2; ++y)
			{
				size_t swapIdx = GetTileSize() - (y + 1);
				char tmp = ImageDataColumn[y];
				ImageDataColumn[y] = ImageDataColumn[swapIdx];
				ImageDataColumn[swapIdx] = tmp;
			}
		}
		return FlippedImage;
	}

	vector<ImageTile> GetTileCandidates() const
	{
		vector<ImageTile> TileCandidates;
		TileCandidates.push_back(*this);
		TileCandidates.push_back(FlipX());
		TileCandidates.push_back(FlipY());
		TileCandidates.push_back(FlipX().FlipY());
		TileCandidates.push_back(Transpose());
		TileCandidates.push_back(Transpose().FlipX());
		TileCandidates.push_back(Transpose().FlipY());
		TileCandidates.push_back(Transpose().FlipX().FlipY());
		return TileCandidates;
	}

	bool CheckAdjacent(ImageTile& other)
	{
		bool bAdjacentTop = true, bAdjacentBottom = true, bAdjacentLeft = true, bAdjacentRight = true;
		for (size_t i = 0; i < GetTileSize(); ++i)
		{
			bAdjacentTop &= ImageDataGrid[i][0] == other.ImageDataGrid[i][GetTileSize() - 1];
			bAdjacentBottom &= ImageDataGrid[i][GetTileSize() - 1] == other.ImageDataGrid[i][0];
			bAdjacentLeft &= ImageDataGrid[0][i] == other.ImageDataGrid[GetTileSize() - 1][i];
			bAdjacentRight &= ImageDataGrid[GetTileSize() - 1][i] == other.ImageDataGrid[0][i];
		}

		if (bAdjacentTop && !NeighborTileIds[ESides::TOP])
		{
			NeighborTileIds[ESides::TOP] = other.TileId;
			other.NeighborTileIds[ESides::BOTTOM] = TileId;
			return true;
		}
		else if (bAdjacentBottom && !NeighborTileIds[ESides::BOTTOM])
		{
			NeighborTileIds[ESides::BOTTOM] = other.TileId;
			other.NeighborTileIds[ESides::TOP] = TileId;
			return true;
		}
		else if (bAdjacentLeft && !NeighborTileIds[ESides::LEFT])
		{
			NeighborTileIds[ESides::LEFT] = other.TileId;
			other.NeighborTileIds[ESides::RIGHT] = TileId;
			return true;
		}
		else if (bAdjacentRight && !NeighborTileIds[ESides::RIGHT])
		{
			NeighborTileIds[ESides::RIGHT] = other.TileId;
			other.NeighborTileIds[ESides::LEFT] = TileId;
			return true;
		}
		return false;
	}

	size_t GetTileSize() const
	{
		return ImageDataGrid.size();
	}
	
	int TileId;
	vector<vector<char>> ImageDataGrid;

	// Mapping of ESides to the neighboring Tile Id
	array<int, NUM_SIDES> NeighborTileIds;
};

template <>
void Run<Day20>(Part part, istream& is, ostream& os)
{
	// Unique Tile ID at each index, vector of candidates for that ID (transposed/flipped)
	vector<vector<ImageTile>> candidateTiles = {};

	// Mapping of Unique Tile ID to the chosen orientation for that tile
	map<int, ImageTile> chosenTilesMap = {};
	
	string arg;
	while (getline(is, arg))
	{
		// Read each image tile
		smatch match;
		if (regex_search(arg, match, RX_TILE_ID))
		{
			ImageTile imageTile = ImageTile(stoi(match.str(1)));
			for (size_t y = 0; y < 10; ++y)
			{
				getline(is, arg);
				for (size_t x = 0; x < arg.size(); ++x)
				{
					if (imageTile.ImageDataGrid.size() <= x)
					{
						imageTile.ImageDataGrid.push_back({});
					}
					imageTile.ImageDataGrid[x].push_back(arg[x]);
				}
			}

			// Let first tile be used to process all others (other tiles will transform to fit)
			if (chosenTilesMap.empty())
			{
				chosenTilesMap.emplace(imageTile.TileId, imageTile);
			}
			else
			{
				candidateTiles.push_back(imageTile.GetTileCandidates());
			}
		}
	}

	// Loop through each row, representing of all possible orientations of each candidate
	auto canidateTilesIt = candidateTiles.end();
	while (!candidateTiles.empty())
	{
		if (canidateTilesIt == candidateTiles.end())
		{
			canidateTilesIt = candidateTiles.begin();
		}
		vector<ImageTile>& candidateTileRow = *canidateTilesIt;
		
		// Try to find an orientation that matches one of the processed candidates
		// This assumes every tile has only one valid match to another tile
		bool bFoundAdjacent = false;
		for (auto& chosenTilePair : chosenTilesMap)
		{
			ImageTile& chosenTile = chosenTilePair.second;
			for (ImageTile& candidateTile : candidateTileRow)
			{
				if (chosenTile.CheckAdjacent(candidateTile))
				{
					// Found a match! Place this orientation as a chosen tile and stop looking
					chosenTilesMap.emplace(candidateTile.TileId, candidateTile);
					bFoundAdjacent = true;
					break;
				}
			}
			if (bFoundAdjacent)
			{
				break;
			}
		}

		// If the one of the candidates matched, remove that row, otherwise try the next row
		if (bFoundAdjacent)
		{
			canidateTilesIt = candidateTiles.erase(canidateTilesIt);
		}
		else
		{
			canidateTilesIt = next(canidateTilesIt);
		}
	}

	// Match up chosen tiles to each-other
	for (auto& itI : chosenTilesMap)
	{
		for (auto& itJ : chosenTilesMap)
		{
			ImageTile& chosenTileI = itI.second;
			ImageTile& chosenTileJ = itJ.second;
			if (chosenTileI.TileId != chosenTileJ.TileId)
			{
				chosenTileI.CheckAdjacent(chosenTileJ);
			}
		}
	}

	// Find the left corner
	const ImageTile* ImageTileTopLeftPtr = nullptr;
	for (auto& chosenTilePair : chosenTilesMap)
	{
		if (chosenTilePair.second.NeighborTileIds[TOP] == 0 && chosenTilePair.second.NeighborTileIds[LEFT] == 0)
		{
			ImageTileTopLeftPtr = &chosenTilePair.second;
			break;
		}
	}

	// Create the final Grid!!!
	const size_t GRID_SIZE = static_cast<size_t>(sqrt(chosenTilesMap.size()));
	vector<vector<const ImageTile*>> imageGrid;
	size_t i = 0, j = 0;
	for (size_t x = 0; x < GRID_SIZE; ++x)
	{
		imageGrid.push_back({});
		vector<const ImageTile*>& imageColumn = imageGrid.back();
		for (size_t y = 0; y < GRID_SIZE; ++y)
		{
			if (x == 0 && y == 0)
			{
				// First corner!
				imageColumn.push_back(ImageTileTopLeftPtr);
			}
			else
			{
				// Find next image ID based on the last added image
				int nextId = (y == 0) ? imageGrid[x-1][y]->NeighborTileIds[RIGHT] : imageGrid[x][y-1]->NeighborTileIds[BOTTOM];
				const ImageTile& nextImage = chosenTilesMap.find(nextId)->second;
				imageColumn.push_back(&nextImage);
			}
		}
	}

	// Print the Grid IDs in order
// 	os << "Grid Order:" << endl;
// 	for (size_t x = 0; x < GRID_SIZE; ++x)
// 	{
// 		for (size_t y = 0; y < GRID_SIZE; ++y)
// 		{
// 			os << imageGrid[x][y]->TileId << "    ";
// 		}
// 		os << endl;
// 	}
// 	os << endl;
	
	if(part == Part01)
	{
		long long multipliedCorers = 
			static_cast<long long>(imageGrid[0][0]->TileId)
			* static_cast<long long>(imageGrid[0][GRID_SIZE - 1]->TileId)
			* static_cast<long long>(imageGrid[GRID_SIZE - 1][0]->TileId)
			* static_cast<long long>(imageGrid[GRID_SIZE - 1][GRID_SIZE - 1]->TileId);
		os << "Multiplied Corners: " << multipliedCorers << endl;

	}
	else if(part == Part02)
	{
		/**
			Monster appears like this:
							  # 
			#    ##    ##    ###
			 #  #  #  #  #  #   
		*/
		array<string, 3> seaMonsterStrings = {
			"                  # ",
			"#    ##    ##    ###",
			" #  #  #  #  #  #   ",
		};
		int monsterHashes = 0;
		vector<vector<char>> seaMonsterGrid;
		for (size_t y = 0; y < seaMonsterStrings.size(); ++y)
		{
			for (size_t x = 0; x < seaMonsterStrings[0].size(); ++x)
			{
				if (seaMonsterGrid.size() <= x)
				{
					seaMonsterGrid.push_back({});
				}
				seaMonsterGrid[x].push_back(seaMonsterStrings[y][x]);
				monsterHashes += static_cast<int>(seaMonsterStrings[y][x] == '#');
			}
		}
		
		// Part 2 requires reading the content of the grid, so turn it into one giant composed Image Tile, stripping the borders of each tile
		size_t TILE_MAX = ImageTileTopLeftPtr->ImageDataGrid.size();
		ImageTile completedImageTile = {0};
		for (size_t tileX = 0; tileX < GRID_SIZE; ++tileX)
		{
			for (size_t tileImageX = 1; tileImageX < TILE_MAX - 1; ++tileImageX)
			{
				completedImageTile.ImageDataGrid.push_back({});
				for (size_t tileY = 0; tileY < GRID_SIZE; ++tileY)
				{
					const ImageTile& imageTile = *imageGrid[tileX][tileY];
					for (size_t tileImageY = 1; tileImageY < TILE_MAX - 1; ++tileImageY)
					{
						completedImageTile.ImageDataGrid.back().push_back(imageTile.ImageDataGrid[tileImageX][tileImageY]);
					}
				}
			}
		}

		// Get all orientations of the composed image
		vector<ImageTile> completedTileCandidates = completedImageTile.GetTileCandidates();
		for (const ImageTile& completedTileCandidate : completedTileCandidates)
		{
			int numMonsters = 0;

			// Try to find the number of sea monsters in every orientation of the completed image
			size_t maxMonsterX = seaMonsterGrid.size();
			size_t maxMonsterY = seaMonsterGrid[0].size();
			for (size_t startX = 0; startX < completedTileCandidate.ImageDataGrid.size() - maxMonsterX; ++startX)
			{
				for (size_t startY = 0; startY < completedTileCandidate.ImageDataGrid.size() - maxMonsterY; ++startY)
				{
					bool bFoundMonster = true;
					for (size_t mX = 0; mX < maxMonsterX; ++mX)
					{
						for (size_t mY = 0; mY < maxMonsterY; ++mY)
						{
							size_t x = startX + mX;
							size_t y = startY + mY;
							
							bFoundMonster &= seaMonsterGrid[mX][mY] == ' ' || (seaMonsterGrid[mX][mY] == completedTileCandidate.ImageDataGrid[x][y]);
						}
					}
					numMonsters += static_cast<int>(bFoundMonster);
				}
			}

			if (numMonsters)
			{
				// Now we have to count all the hashes that aren't the sea monster because this question is the devil
				int numHashes = 0;
				for (size_t x = 0; x < completedTileCandidate.ImageDataGrid.size(); ++x)
				{
					for (size_t y = 0; y < completedTileCandidate.ImageDataGrid.size(); ++y)
					{
						numHashes += static_cast<int>(completedTileCandidate.ImageDataGrid[x][y] == '#');
					}
				}
				os << "Total Monsters: " << numMonsters << endl;
				os << "Total Hashes: " << numHashes << endl;
				os << "Total \"Waves\": " << numHashes - (numMonsters * monsterHashes) << endl;
// 				for (size_t x = 0; x < completedTileCandidate.ImageDataGrid.size(); ++x)
// 				{
// 					for (size_t y = 0; y < completedTileCandidate.ImageDataGrid.size(); ++y)
// 					{
// 						os << completedTileCandidate.ImageDataGrid[x][y];
// 					}
// 					os << endl;
// 				}
//				os << endl;
			}
		}
	}
}