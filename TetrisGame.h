#ifndef _Tetris_Game
#define _Tetris_Game
#include "Shape.h"
#include <iostream>
using namespace std;

#define ROW 15
#define COLUMN 10


class TetrisGame
{
	enum {ESC=27,SPACE_key=32, s_key=115, S_key=83, ONE=49, TWO=50, THREE=51, FOUR=52, DOWN_KEY = 80, LEFT_KEY=75, UP_KEY=72, RIGHT_KEY=77};
	Shape currentShape;
	char keyboards[11];
	int Board[COLUMN][ROW];
public:
	void displayBorder();
	void runGame();
	void initGame();
	int checkKeys(char ch);
	void setKeys(){
		keyboards[0] = DOWN_KEY; // down arrow
		keyboards[1] = LEFT_KEY; // left arrow
		keyboards[2] = UP_KEY; // up arrow
		keyboards[3] = RIGHT_KEY; // right arrow
		keyboards[4] = SPACE_key; 
		keyboards[5] = s_key; 
		keyboards[6] = S_key; 
		keyboards[7] = ONE;
		keyboards[8] = TWO;
		keyboards[9] = THREE;
		keyboards[10] = FOUR;

	}
	int randomNum(){
		int res = rand() % 100;
		if (res < 70)
			res = rand() % 2 + 10;
		else
			res = rand() % 2 + 12;
		return res;
	}

	void printMenu();
};

#endif