#ifndef _Tetris_Game
#define _Tetris_Game
#include "ScoreBar.h"
#include <iostream>
using namespace std;

#define ROW 15
#define COLUMN 10


class TetrisGame
{
	enum {ESC=27,SPACE_key=32, s_key=115, S_key=83, ONE=49, TWO=50, THREE=51, FOUR=52, DOWN_KEY = 80, LEFT_KEY=75, UP_KEY=72, RIGHT_KEY=77, PAUSED, PLAYING};
	Shape currentShape;
	char keyboards[11];
	int Board[COLUMN][ROW], gameStarted = 0;
public:
	void setGameStarted();
	void displayBorder();
	void runGame(TetrisBoard& board, Score& scoreStatus);
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
	void printGameOver() {
		setTextColor(LIGHTCYAN);
		gotoxy(0, 20);
		cout << "     _____                         ____                 " << endl;
		cout << "    / ____|                       / __ \\                " << endl;
		cout << "   | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ " << endl;
		cout << "   | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|" << endl;
		cout << "   | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   " << endl;
		cout << "    \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   " << endl;
		Sleep(5000);
	}
	bool checkExit(char keyEntered) {
		if (keyEntered == '9') {
			printGameOver();
			return true;
		}
		return false;
	}

	bool checkPause(char keyEntered) {
		if (keyEntered == '2') {
			Sleep(1000);
			return true;
		}
		return false;
	}

	void newRound(int& isBombed, int& tempTime, int& timeInterval, TetrisBoard& board, int& minY, int& maxY, Score& scoreStatus, int& howManyBombed, unsigned long int& whichShape);

	void changeSpeed(char indicator, int& tempTime, int& timeInterval, Score& scoreStatus) {

		if (indicator == THREE) // increase speed
		{
			tempTime -= 300;
			scoreStatus.increaseSpeed();
		}
		else // decrease speed
		{
			tempTime += 300;
			scoreStatus.decreaseSpeed();
		}
		timeInterval = 800 + tempTime;
		scoreStatus.printSpeed();
	}

	void MenuControl(char keyPressed, TetrisBoard& board, Score& scoreStatus);

	void continueBlink();

};

#endif