#include "Board.h"
#include <Windows.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

string player1 = "X";
string player2 = "Y";

//fuck you

Board::Board(int size)
{
	boardSize = size;
	blankBoard();
	buildPathways();
}

void Board::buildPathways()
{
	buildAllTwoPointPairs();
	removeDuplicatePairs();
	findColinears();
	removeBlankPaths();
}

void Board::blankBoard()
{
	vector< vector<int> > tempSpaces(boardSize, vector<int>(boardSize));
	int count = 0;
	for (int i = 0; i < tempSpaces.size(); i++)
		for (int j = 0; j < tempSpaces[i].size(); j++)
		{
			count++;
			tempSpaces[i][j] = count;
		}
	spaces = tempSpaces;
}

void Board::displayBoard()
{
	cout << endl;
	int spaceWidth = 2;
	cout << " ";
	for (int i = 1; i < boardSize*(spaceWidth+1); i++)
		if (i % (spaceWidth + 1) == 0)
			cout << char(179);
		else
			cout << " ";
	cout << " " << endl;
	int counter = 0;
	for (int i = 0; i < spaces.size(); i++)
	{
		cout << " ";
		for (int j = 0; j < spaces[i].size(); j++)
			if (spaces[i][j] != 0)
			{
				if (spaces[i][j] < 0)
				{
					if (spaces[i][j] == -1)
						cout << setw(spaceWidth) << player1;
					else if (spaces[i][j] == -2)
						cout << setw(spaceWidth) << player2;
					if (j!= spaces.size() - 1)
					cout << char(179);
					continue;
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
				cout << setw(spaceWidth) << spaces[i][j];
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				if (j!= spaces.size() - 1)
					cout << char(179);
			}
			else
			{
				counter++;
				cout << setw(spaceWidth) << " ";
				if (j != spaces[i].size() - 1)
					cout << char (179);
			}
		if (i < spaces.size() - 1)
		{
			cout << " " << endl << char(196);
			for (int k = 1; k < boardSize*(spaceWidth+1); k++)
				if (k % (spaceWidth + 1) == 0)
					cout << char(197);
				else
					cout << char(196);
			cout << char(196) << endl;
		}
	}
	cout << " " << endl << " ";
	for (int i = 1; i < boardSize*(spaceWidth+1); i++)
		if (i % (spaceWidth + 1) == 0)
			cout << char(179);
		else
			cout << " ";
	cout << " " << endl;
	cout << endl;
}

void Board::buildAllTwoPointPairs()
{
	for (int x = 0; x < spaces.size(); x++)
		for (int y = 0; y < spaces[x].size(); y++)
			for (int a = 0; a < spaces.size(); a++)
				for (int b = 0; b < spaces[a].size(); b++)
				{
					if (x == a && y == b)
						continue;
					thePoint one = *(new thePoint);
					one.X = x;
					one.Y = y;
					thePoint two = *(new thePoint);
					two.X = a;
					two.Y = b;
					pointPair thePair = *(new pointPair);
					thePair.one = one;
					thePair.two = two;
					pairs.push_back(thePair);
				}
}

void Board::removeDuplicatePairs()
{
	for (int i = 0; i < pairs.size(); i++)
		for (int j = i + 1; j < pairs.size(); j++)
			if ((pairs[i].one.X == pairs[j].one.X && pairs[i].one.Y == pairs[j].one.Y && pairs[i].two.X == pairs[j].two.X && pairs[i].two.Y == pairs[j].two.Y)
				|| (pairs[i].one.X == pairs[j].two.X && pairs[i].one.Y == pairs[j].two.Y && pairs[i].two.X == pairs[j].one.X && pairs[i].two.Y == pairs[j].one.Y)
				|| (areOnSameLine(pairs[i].one, pairs[i].two, pairs[j].one) && areOnSameLine(pairs[i].one, pairs[i].two, pairs[j].two)))
			{
				pairs.erase(pairs.begin() + j);
				j--;
				continue;
			}
}

bool Board::areOnSameLine(thePoint one, thePoint two, thePoint three)
{
	if (two.X-one.X == 0)
		return (two.X == three.X);
	double slope = (double)(two.Y-one.Y)/(double)(two.X-one.X);
	double intercept = one.Y - (double)slope*one.X;
		if (three.Y == (double)slope*three.X + (double)intercept)
			return true;
		else
			return false;
}

void Board::findColinears()
{
	for (int i = 0; i < pairs.size(); i++)
		for (int c = 0; c < spaces.size(); c++)
			for (int d = 0; d < spaces[c].size(); d++)
			{
				if ((pairs[i].one.X == c && pairs[i].one.Y == d) || (pairs[i].two.X == c && pairs[i].two.Y == d))
					continue;
				thePoint three = *(new thePoint);
				three.X = c;
				three.Y = d;
				if (areOnSameLine(pairs[i].one, pairs[i].two, three))
					pairs[i].colinears.push_back(three);
			}
}

void Board::removeBlankPaths()
{
	for (int i = 0; i < pairs.size(); i++)
		if (pairs[i].colinears.size() < boardSize - 2)
		{
			pairs.erase(pairs.begin() + i);
			i--;
			continue;
		}
}

void Board::displayColinears()
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
			cout << ",";
		int j;
		for (j = 0; j < pairs[i].colinears.size() - 1; j++)
			cout << "(" << pairs[i].colinears[j].X << "," << pairs[i].colinears[j].Y << "),";
		cout << "(" << pairs[i].colinears[j].X << "," << pairs[i].colinears[j].Y << ")" << endl;
	}
	cout << endl;
	system("pause");
}

bool Board::makeMove(int player, int space)
{
	for (int i = 0; i < spaces.size(); i++)
		for (int j = 0; j < spaces[i].size(); j++)
			if (spaces[i][j] == space)
			{
				spaces[i][j] = -player;
				return true;
			}
	return false;
}

bool Board::checkIfWin(int player)
{
	for (int i = 0; i < pairs.size(); i++)
		if (spaces[pairs[i].one.X][pairs[i].one.Y] == -player && spaces[pairs[i].two.X][pairs[i].two.Y] == -player)
		{
			if (pairs[i].colinears.empty())
				return true;
			bool win = true;
			for (int j = 0; j < pairs[i].colinears.size(); j++)
				if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] != -player)
					win = false;
			if (win)
				return true;
		}
	return false;
}

int Board::bestMove(int player)
{
	vector< vector<int> > spacesCount = spaces;
	for (int i = 0; i < spacesCount.size(); i++)
		for (int j = 0; j < spacesCount[i].size(); j++)
			spacesCount[i][j] = 0;

	int biggestIndex = 0;
	int biggestCount = 0;
	for (int i = 0; i < pairs.size(); i++)
	{
		int otherPlayer = 0;
		int count = 0;
		spacesCount[pairs[i].one.X][pairs[i].one.Y]++;
		spacesCount[pairs[i].two.X][pairs[i].two.Y]++;
		if (spaces[pairs[i].one.X][pairs[i].one.Y] == -player)
			count++;
		if (spaces[pairs[i].one.X][pairs[i].one.Y] != -player && spaces[pairs[i].one.X][pairs[i].one.Y] < 0)
			otherPlayer++;
		if (spaces[pairs[i].two.X][pairs[i].two.Y] == -player)
			count++;
		if (spaces[pairs[i].two.X][pairs[i].two.Y] != -player && spaces[pairs[i].two.X][pairs[i].two.Y] < 0)
			otherPlayer++;
		if (!pairs[i].colinears.empty())
			for (int j = 0; j < pairs[i].colinears.size(); j++)
			{
				spacesCount[pairs[i].colinears[j].X][pairs[i].colinears[j].Y]++;
				if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] == -player)
					count++;
				if (spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] != -player && spaces[pairs[i].colinears[j].X][pairs[i].colinears[j].Y] < 0)
					otherPlayer++;
			}
		if (count > biggestCount && otherPlayer == 0)
		{
			biggestIndex = i;
			biggestCount = count;
		}
	}
	//for (int i = 0; i < spacesCount.size(); i++)
	//{
	//	for (int j = 0; j < spacesCount[i].size(); j++)
	//		cout << spacesCount[i][j] << " ";
	//	cout << endl;
	//}
	int move = 0;
	int bestCount = 0;
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
			if (spaces[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y] > 0 && spacesCount[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y] > bestCount)
			{
				move = spaces[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y];
				bestCount = spacesCount[pairs[biggestIndex].colinears[i].X][pairs[biggestIndex].colinears[i].Y];
			}
	}
	if (move == 0)
		move = 0; // most-referenced position?
	return move;
}

