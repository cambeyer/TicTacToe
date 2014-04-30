#include "Board.h"
#include <Windows.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

string player1 = "X";
string player2 = "O";

Board::Board(int size)													//constructor
{
	boardSize = size;
	blankBoard();
	buildPathways();
}

void Board::buildPathways()														//figure out all of the possible winning combinations
{
	buildAllTwoPointPairs();													//build all possible combinations of two points (minimum required to define a line)
	removeDuplicatePairs();														//figure out which point combinations are actually the same line
	findColinears();															//from all the rest of the points on the board, figure out which spaces are on the same line as each pair of points from above
	removeBlankPaths();															//any pairs of points that didn't end up having [dimension of the board] spaces aren't valid solutions to the board
}

void Board::blankBoard()
{
	vector< vector<int> > tempSpaces(boardSize, vector<int>(boardSize));		//vector of vectors for ease of allocation
	int count = 0;
	for (int i = 0; i < tempSpaces.size(); i++)
	{
		for (int j = 0; j < tempSpaces[i].size(); j++)
		{
			count++;
			tempSpaces[i][j] = count;											//assign spaces numbers to choose from in the game
		}
	}
	spaces = tempSpaces;														//replace the board's vector of vectors with this temp one
}

void Board::displayBoard()														//print the board to the screen using ASCII characters
{
	cout << endl;
	int spaceWidth = 2;
	cout << " ";
	for (int i = 1; i < boardSize*(spaceWidth+1); i++)
	{
		if (i % (spaceWidth + 1) == 0)
		{
			cout << char(179);													//using the spaceWidth global variable, figure out if it's time to put an alternate character rather than just a blank
		}
		else
		{
			cout << " ";
		}
	}
	cout << " " << endl;
	int counter = 0;
	for (int i = 0; i < spaces.size(); i++)
	{
		cout << " ";															//put a little padding at the left-side of the board so it looks good
		for (int j = 0; j < spaces[i].size(); j++)
		{
			if (spaces[i][j] != 0)
			{
				if (spaces[i][j] < 0)
				{
					if (spaces[i][j] == -1)										//if the space is occupied by player 1, print their character out instead of the space number
					{
						cout << setw(spaceWidth) << player1;
					}
					else if (spaces[i][j] == -2)
					{
						cout << setw(spaceWidth) << player2;					//if the space is occupied by player 2, print their character out instead of the space number
					}
					if (j!= spaces.size() - 1)
					{
					cout << char(179);
					}
					continue;
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);	//change color to green to output the spaces to choose, so they aren't so in-your-face
				cout << setw(spaceWidth) << spaces[i][j];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);	//back to white
				if (j != spaces.size() - 1)
				{
					cout << char(179);
				}
			}
			else
			{
				counter++;
				cout << setw(spaceWidth) << " ";
				if (j != spaces[i].size() - 1)
				{
					cout << char (179);
				}
			}
		}
		if (i < spaces.size() - 1)
		{
			cout << " " << endl << char(196);
			for (int k = 1; k < boardSize*(spaceWidth+1); k++)
			{
				if (k % (spaceWidth + 1) == 0)									//using global variable spaceWidth to determine where to put crosses instead of pipes
				{
					cout << char(197);
				}
				else
				{
					cout << char(196);
				}
			}
			cout << char(196) << endl;
		}
	}
	cout << " " << endl << " ";
	for (int i = 1; i < boardSize*(spaceWidth+1); i++)
	{
		if (i % (spaceWidth + 1) == 0)											//determine where to put pipes instead of spaces
		{
			cout << char(179);
		}
		else
		{
			cout << " ";
		}
	}
	cout << " " << endl;
	cout << endl;
}

void Board::buildAllTwoPointPairs()												//build all possible combinations of two points (minimum required to define a line)
{
	for (int x = 0; x < spaces.size(); x++)
	{
		for (int y = 0; y < spaces[x].size(); y++)								//for each X/Y...
		{
			for (int a = 0; a < spaces.size(); a++)
			{
				for (int b = 0; b < spaces[a].size(); b++)						//find every other X/Y...
				{
					if (x == a && y == b)										//if they're the same point, don't count it as a pair
					{
						continue;
					}
					if (sqrt((a - x)*(a - x) + (b - y)*(b - y)) >= 2)			//if points are more than 2 spaces from each other, they are surely duplicates
					{
						continue;
					}
					thePoint one = *(new thePoint);								//push the X/Y to a new Point structure
					one.X = x;
					one.Y = y;
					thePoint two = *(new thePoint);
					two.X = a;
					two.Y = b;
					pointPair thePair = *(new pointPair);						//push the new pairs to a pointpair structure
					thePair.one = one;
					thePair.two = two;
					pairs.push_back(thePair);
				}
			}
		}
	}
}

void Board::removeDuplicatePairs()												//figure out which point combinations are actually the same line
{
	for (int i = 0; i < pairs.size(); i++)										//for each pair...
	{
		for (int j = i + 1; j < pairs.size(); j++)								//go through every other pair...
		{
			if ((pairs[i].one.X == pairs[j].one.X && pairs[i].one.Y == pairs[j].one.Y && pairs[i].two.X == pairs[j].two.X && pairs[i].two.Y == pairs[j].two.Y)
				|| (pairs[i].one.X == pairs[j].two.X && pairs[i].one.Y == pairs[j].two.Y && pairs[i].two.X == pairs[j].one.X && pairs[i].two.Y == pairs[j].one.Y)
				|| (areOnSameLine(pairs[i].one, pairs[i].two, pairs[j].one) && areOnSameLine(pairs[i].one, pairs[i].two, pairs[j].two)))
			{
				pairs.erase(pairs.begin() + j);									//if point1 = point1 from pair to pair and point2 = point2, or point1 = point2 and point2 = point1, they are the same pair
				j--;															//if the points are on the same line, then one pair is a duplicate
				continue;
			}
		}
	}
}

bool Board::areOnSameLine(thePoint one, thePoint two, thePoint three)			//determine if three points are on the same line (two points will ALWAYS be on a line, but not necessarily three)
{
	if (two.X-one.X == 0)														//if the X-values are the same, then we don't want to divide by zero later with the slopes...
	{
		return (two.X == three.X);												//so if the third point's X-value is zero too, they are on the same line without checking further
	}
	double slope = (double)(two.Y-one.Y)/(double)(two.X-one.X);					//calculate using slope-intercept whether the third point lies on a line with the first and second
	double intercept = one.Y - (double)slope*one.X;
	if (three.Y == (double)slope*three.X + (double)intercept)					//if the third point's values come out matching the expected, it is on a line
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Board::findColinears()														//for each pair, and all remaining points, figure out which spaces are on the same line as each pair of points
{
	for (int i = 0; i < pairs.size(); i++)										//for all pairs...
	{
		for (int c = 0; c < spaces.size(); c++)									//go through all Xs...
		{
			for (int d = 0; d < spaces[c].size(); d++)							//... and Ys on the board
			{
				if ((pairs[i].one.X == c && pairs[i].one.Y == d) || (pairs[i].two.X == c && pairs[i].two.Y == d))
				{
					continue;													//if the point is already in the pair, we don't want it listed as a colinear
				}
				thePoint three = *(new thePoint);
				three.X = c;
				three.Y = d;
				if (areOnSameLine(pairs[i].one, pairs[i].two, three))
				{
					pairs[i].colinears.push_back(three);						//if it is in line with the existing point pair, then it's colinear
				}
			}
		}
	}
}

void Board::removeBlankPaths()
{
	for (int i = 0; i < pairs.size(); i++)
	{
		if (pairs[i].colinears.size() < boardSize - 2)
		{
			pairs.erase(pairs.begin() + i);										//if a list of colinears never developed for a point pair, it's not a valid winning pathway
			i--;
			continue;
		}
	}
}

void Board::displayColinears()													//diagnostic function to output all winning pathways
{
	cout << endl << "DIAGNOSTIC: ALL WINNING COMBINATIONS" << endl << endl;
	for (int i = 0; i < pairs.size(); i++)
	{
		cout << "(" << pairs[i].one.X << "," << pairs[i].one.Y << "),(" << pairs[i].two.X << "," << pairs[i].two.Y << ")";
		if (pairs[i].colinears.empty())
		{
			cout << endl;
			continue;
		}
		else
		{
			cout << ",";
		}
		int j;
		for (j = 0; j < pairs[i].colinears.size() - 1; j++)
		{
			cout << "(" << pairs[i].colinears[j].X << "," << pairs[i].colinears[j].Y << "),";		//print out all the point pairs and all their colinears, one point pair per line
		}
		cout << "(" << pairs[i].colinears[j].X << "," << pairs[i].colinears[j].Y << ")" << endl;
	}
	cout << endl;
	system("pause");
}

bool Board::makeMove(int player, int space)										//commit a move to a space in the board with that number
{
	for (int i = 0; i < spaces.size(); i++)
	{
		for (int j = 0; j < spaces[i].size(); j++)
		{
			if (spaces[i][j] == space)
			{
				spaces[i][j] = -player;											//replace the space in the board with the negative player number to keep track of who has what
				return true;													//the move was successful
			}
		}
	}
	return false;																//if no such number exists in the board, then the move is not valid
}

bool Board::checkIfWin(int player)												//if one player has all of the points represented by a point pair and all its colinears, they have won
{
	for (int i = 0; i < pairs.size(); i++)
	{
		if (spaces[pairs[i].one.X][pairs[i].one.Y] == -player && spaces[pairs[i].two.X][pairs[i].two.Y] == -player)	//check the point pair points individually
		{
			if (pairs[i].colinears.empty())
			{
				return true;
			}
			bool win = true;
			for (int j = 0; j < pairs[i].colinears.size(); j++)					//then check all the colinears
			{
				if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] != -player)
				{
					win = false;												//if anything isn't the player's, then they haven't won
				}
			}
			if (win)
			{
				return true;													//all things in an entire point pair are taken by one player
			}
		}
	}
	return false;
}

bool Board::checkIfTie()														//if all winning pathways are a mix of both players, the game cannot be won
{
	for (int i = 0; i < pairs.size(); i++)
	{
		int tempPlayer = 0;
		if (spaces[pairs[i].one.X][pairs[i].one.Y] < 0 && spaces[pairs[i].two.X][pairs[i].two.Y] < 0 && spaces[pairs[i].one.X][pairs[i].one.Y] != spaces[pairs[i].two.X][pairs[i].two.Y])	//if both spaces are occupied, and occupied by different players
		{
			continue;															//this pathway cannot be won
		}
		if (pairs[i].colinears.empty())
		{
			return false;														//if there aren't any colinears to worry about, and the check above didn't fail, then there is at least one good path left 
		}
		if (spaces[pairs[i].one.X][pairs[i].one.Y] < 0)							//if the first point of the point pair is occupied, preliminarily compare off that
		{
			tempPlayer = spaces[pairs[i].one.X][pairs[i].one.Y];
		}
		if (spaces[pairs[i].two.X][pairs[i].two.Y] < 0)							//if the second point of the point pair is occupied, preliminarily compare off that
		{
			tempPlayer = spaces[pairs[i].two.X][pairs[i].two.Y];
		}
		bool doContinue = false;
		for (int j = 0; j < pairs[i].colinears.size(); j++)						//then check all the colinears
		{
			if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] < 0 && tempPlayer == 0)	//if the preliminary compare wasn't set and this colinear is occupied, compare off that
			{
				tempPlayer = spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y];
			}
			if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] < 0 && spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] != tempPlayer)	//if the colinear is occupied and is different than something already in the row, the pathway is unwinnable
			{
				doContinue = true;
				break;
			}
		}
		if (doContinue)
		{
			continue;															//added handler because of the nested for loop
		}
		else
		{
			return false;														//there is at least one complete path without conflicting players present
		}
	}
	return true;																//if all else fails, it must be a tie game
}

int Board::movesRemaining()														//how many openings are left on the board to make moves in?
{
	int counter = 0;
	for (int i = 0; i < spaces.size(); i++)
	{
		for (int j = 0; j < spaces[i].size(); j++)
		{
			if (spaces[i][j] > 0)
			{
				counter++;														//anything greater than zero is still open
			}
		}
	}
	return counter;																//return the count
}

int Board::bestMove(int player)													//hidden feature that helps determine the best aggressive move (no defense capabilities)
{
	vector< vector<int> > spacesCount = spaces;									//each space will have a count of how many winning pathways it is a part of
	for (int i = 0; i < spacesCount.size(); i++)
	{
		for (int j = 0; j < spacesCount[i].size(); j++)
		{
			spacesCount[i][j] = 0;												//initialize all spaces counts to zero
		}
	}

	int biggestIndex = 0;
	int biggestCount = 0;
	for (int i = 0; i < pairs.size(); i++)
	{
		int otherPlayer = 0;
		int count = 0;
		spacesCount[pairs[i].one.X][pairs[i].one.Y]++;							//increment the counter for the first point in the pair
		spacesCount[pairs[i].two.X][pairs[i].two.Y]++;							//increment the counter for the second point in the pair
		if (spaces[pairs[i].one.X][pairs[i].one.Y] == -player)
		{
			count++;
		}
		if (spaces[pairs[i].one.X][pairs[i].one.Y] != -player && spaces[pairs[i].one.X][pairs[i].one.Y] < 0)
		{
			otherPlayer++;
		}
		if (spaces[pairs[i].two.X][pairs[i].two.Y] == -player)
		{
			count++;
		}
		if (spaces[pairs[i].two.X][pairs[i].two.Y] != -player && spaces[pairs[i].two.X][pairs[i].two.Y] < 0)
		{
			otherPlayer++;
		}
		if (!pairs[i].colinears.empty())
		{
			for (int j = 0; j < pairs[i].colinears.size(); j++)
			{
				spacesCount[pairs[i].colinears[j].X][pairs[i].colinears[j].Y]++;	//increment the counter for all the colinears
				if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] == -player)
				{
					count++;
				}
				if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] != -player && spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] < 0)
				{
					otherPlayer++;
				}
			}
		}
		if (count > biggestCount && otherPlayer == 0)
		{
			biggestIndex = i;
			biggestCount = count;
		}
	}
	int move = 0;
	int bestCount = 0;
	//go through all point pairs and colinears to find the one that is referenced the most and use that move
	if (spaces[pairs[biggestIndex].one.X][pairs[biggestIndex].one.Y] > 0 && spacesCount[pairs[biggestIndex].one.X][pairs[biggestIndex].one.Y] > bestCount)
	{
		move = spaces[pairs[biggestIndex].one.X][pairs[biggestIndex].one.Y];
		bestCount = spacesCount[pairs[biggestIndex].one.X][pairs[biggestIndex].one.Y];
	}
	if (spaces[pairs[biggestIndex].two.X][pairs[biggestIndex].two.Y] > 0 && spacesCount[pairs[biggestIndex].two.X][pairs[biggestIndex].two.Y] > bestCount)
	{
		move = spaces[pairs[biggestIndex].two.X][pairs[biggestIndex].two.Y];
		bestCount = spacesCount[pairs[biggestIndex].two.X][pairs[biggestIndex].two.Y];
	}
	if (!pairs[biggestIndex].colinears.empty())
	{
		for (int i = 0; i < pairs[biggestIndex].colinears.size(); i++)
		{
			if (spaces[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y] > 0 && spacesCount[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y] > bestCount)
			{
				move = spaces[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y];
				bestCount = spacesCount[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y];
			}
		}
	}
	if (move == 0)
	{
		move = 0; // most-referenced position?
	}
	return move;
}

