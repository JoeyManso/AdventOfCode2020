#include "Days.h"
#include <list>
#include <map>
#include <vector>

typedef pair<list<int>, list<int>> PlayerDecks;

/** Plays the card game, returning the winner and their cards  */
pair<size_t, list<int>> PlayCombat(PlayerDecks playerDecks, bool bRecursiveRules = false)
{
	vector<PlayerDecks> previousRounds;

	// Game continues until a win condition is met or one player's deck is exhausted
	while (playerDecks.first.size() && playerDecks.second.size())
	{
		if (find(previousRounds.begin(), previousRounds.end(), playerDecks) != previousRounds.end())
		{
			// Player 1 wins if any rounds repeat
			return pair<size_t, list<int>>(1, playerDecks.first);
		}
		else
		{
			if (bRecursiveRules)
			{
				// Track previous decks (before modification) to avoid infinite loop during recursion play
				previousRounds.push_back(playerDecks);
			}

			// Draw a card from each deck
			int player1Card = playerDecks.first.front();
			playerDecks.first.pop_front();
			int player2Card = playerDecks.second.front();
			playerDecks.second.pop_front();

			// Determine the round's winner
			size_t roundWinner = INT_MAX;

			if (bRecursiveRules && playerDecks.first.size() >= player1Card && playerDecks.second.size() >= player2Card)
			{
				// If both players have at least as many cards remaining in their deck as the value of the card they just drew
				// the winner of the round is determined by playing a new game of Recursive Combat
				// The new decks need to trim to a count equal to the value of their drawn card
				PlayerDecks newDecks = playerDecks;
				newDecks.first.resize(player1Card);
				newDecks.second.resize(player2Card);
				roundWinner = PlayCombat(newDecks, bRecursiveRules).first;
			}
			else
			{
				// Otherwise, winner is determined by higher value card
				roundWinner = (player1Card > player2Card) ? 1 : 2;
			}
			
			if (roundWinner == 1)
			{
				playerDecks.first.push_back(player1Card);
				playerDecks.first.push_back(player2Card);
			}
			else
			{
				playerDecks.second.push_back(player2Card);
				playerDecks.second.push_back(player1Card);
			}
		}
	}
	return playerDecks.first.size() ? pair<size_t, list<int>>(1, playerDecks.first) : pair<size_t, list<int>>(2, playerDecks.second);
}

template <>
void Run<Day22>(Part part, istream& is, ostream& os)
{
	// Read in the starting decks
	PlayerDecks playerDecks;
	
	size_t i = -1;
	string arg;
	while (getline(is, arg))
	{
		if (!arg.empty())
		{
			if (arg.substr(0, 6) == "Player")
			{
				++i;
			}
			else if (i == 0)
			{
				playerDecks.first.push_back(stoi(arg));
			}
			else
			{
				playerDecks.second.push_back(stoi(arg));
			}
		}
	}

	pair<size_t, list<int>> winningPlayerDeck = PlayCombat(playerDecks, part == Part02);
	size_t totalScore = 0;
	size_t cardMultiplier = winningPlayerDeck.second.size();
	for (int card : winningPlayerDeck.second)
	{
		totalScore += card * cardMultiplier;
		--cardMultiplier;
	}
	os << "Player " << winningPlayerDeck.first << " wins with " << totalScore << " points!" << endl;
}