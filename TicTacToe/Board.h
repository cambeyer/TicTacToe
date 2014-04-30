#ifndef BOARD_H
#define BOARD_H

#include <vector>

using namespace std;

struct thePoint
{
	int X;
	int Y;
};

struct pointPair {
	thePoint one;
	thePoint two;
	vector<thePoint> colinears;
};

class Board
{
public:
   Board(int size = 3);
   void blankBoard();
   void displayBoard();
   void buildPathways();
   void buildAllTwoPointPairs();
   void removeDuplicatePairs();
   bool areOnSameLine(thePoint, thePoint, thePoint);
   void findColinears();
   void removeBlankPaths();
   void displayColinears();
   bool makeMove(int, int);
   bool checkIfWin(int);
   bool checkIfTie();
   int movesRemaining();
   int bestMove(int);
private:
   int boardSize;
   vector< vector<int> > spaces;
   vector< pointPair > pairs;
};

#endif
