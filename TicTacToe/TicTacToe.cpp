#include <iomanip>
#include <iostream>
#include <Windows.h>
#include <string>
#include "Board.h"

using namespace std;

//Simply output the welcome screen to the user that shall describe the game and instructions.
void displayWelcome()
{
	cout << "TicTacToe Version 1.0" << endl 
		<< "Created By: Cameron Beyer, Dominic Bucchare, and Dean Perkins" << endl 
		<< endl
		<< "The rules remain just like any normal Tic Tac Toe game and are as follows:" << endl
		<< "The goal of Tic Tac Toe is to be the first player to get three in a row on" << endl 
		<< "a 3x3 grid, or four in a row in a 4x4 grid, etc." << endl 
		<< endl
		<< "In the following screen you shall define the size of the board." << endl
		<< "From there the board will be generated for you and you can play :D." << endl 
		<< endl
		<< "All each player must do is simply enter the location on the board (as the " << endl
		<< "number shown), to place their piece. A winner will be announced once someone" << endl
		<< "successfully gets their pieces in a row, a tie shall be announced if it occurs." << endl
		<< endl
		<< "Enjoy the game :D" << endl;
	system("pause");
	system("cls"); //Clears the screen at the end
}

//Gets the determining size from the user for the Board to be initialized.
Board chooseBoard()
{
	int size = 0;
	string sizeInput = "";
	while (size < 2 || size > 10) //Maximum of ten was created do to the console window set size
	{
		cout << "Please enter the desired size of the board (min 2, max 10): ";
		cin >> sizeInput;
		size = atoi(sizeInput.c_str());
	}
	cout << endl << "Please wait; generating board!" << endl;
	Board theBoard(size);
	return theBoard;
}

//Runs the "checkIfWin" function of the board and outputs the final result if someone wins or players tie.
bool checkWin(int player, Board theBoard)
{
	if (theBoard.checkIfWin(player))
	{
		cout << "Player " << player << " wins!!!" << endl;
		return true;
	}
	else if (theBoard.checkIfTie())
	{
		cout << "Cat's Game (Players have tied)!" << endl;
		return true; 
	}
	else
	{
		return false;
	}
}

//Function to play the game.
bool playGame(Board theBoard)
{
	//theBoard.displayColinears(); //debug line; shows all possible win paths
	system("cls");

	int player = 1;
	theBoard.displayBoard();
	while (!checkWin(player, theBoard))
	{
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
	string playAgain = "";	//For when the user is asked if they want to play again.
	while (playAgain != "y" && playAgain != "Y" && playAgain != "n" && playAgain != "N")
	{
		cout << "Game over; play again? (y/n): ";
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
	//The pipeling for the entire game (calls to functions in the proper order).
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