#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <string>
#include "Board.h"

using namespace std;

void displayWelcome()
{
	cout << "Welcome!!" << endl << endl;
	system("pause");
	system("cls");
}

Board chooseBoard()
{
	int size = 0;
	string sizeInput = "";
	while (size < 2 || size > 10)
	{
		cout << "Please enter the desired size of the board (min 2, max 10): ";
		cin >> sizeInput;
		size = atoi(sizeInput.c_str());
	}
	cout << endl << "Please wait; generating board!" << endl;
	Board theBoard(size);
	return theBoard;
}

bool checkWin(int player, Board theBoard)
{
	if (theBoard.checkIfWin(player))
	{
		cout << "Player " << player << " wins!!!" << endl;
		system("pause");
		return true;
	}
	else
	{
		cout << "Player " << player << " has not won." << endl;
		return false;
	}
}

bool playGame(Board theBoard)
{
	theBoard.displayColinears(); //debug line; shows all possible win paths
	system("cls");

	/* ABOUT TO SIMULTATE PLAYER 1 CHOOSING POSITIONS 1, 2, AND 3! TO SEE OUTPUT, SET BOARD SIZE TO 3 WHEN PROMPTED */
	int player = 1;
	while (!checkWin(player, theBoard))
	{
		theBoard.displayBoard();
		string move = "";
		int theMove = -1;
		while (theMove < 0)
		{
			cout << "Player " << player << ", please make a move: ";
			cin >> move;
			theMove = atoi(move.c_str());
		}
		if (theMove == 0)
		{
			theBoard.makeMove(player, theBoard.bestMove(player));
		}
		else
		{
			theBoard.makeMove(player, theMove);
		}
		theBoard.displayBoard();
		if (checkWin(player, theBoard))
		{
			break;
		}
		if (player == 1)
		{
			player = 2;
		}
		else
		{
			player = 1;
		}
	}
	string playAgain = "";
	while (playAgain != "y" && playAgain != "Y" && playAgain != "n" && playAgain != "N")
	{
		cout << "Play again? (y/n): " << endl;
		cin >> playAgain;
		if (playAgain == "y" || playAgain == "Y")
		{
			return true;
		}
	}
	return false;
}

int main()
{
	displayWelcome();
	bool play = true;
	Board theBoard;
	while (play)
	{
		theBoard = chooseBoard();
		system("cls");
		play = playGame(theBoard);
		system("cls");
	}
	cout << "Thanks for playing TicTacToe!" << endl << endl;
	system("pause");
}