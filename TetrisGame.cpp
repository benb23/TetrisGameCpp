#include "TetrisGame.h"
#include "ScoreBar.h"
#include "GoToXY.h"
#include "Config.h"
#include "TetrisBoard.h"
#include <thread>
#include <chrono>

#define invalid_Key -1

int TetrisGame:: MenuControl(char keyPressed, TetrisBoard& board, Score& scoreStatus) {

	switch (keyPressed) {
	
	case '1':
		setGameStarted();
		return runGame(board, scoreStatus);
		break;

	case '2':
		if (gameStarted) {
			setGameStarted(); //prints the normal continue color
			return runGame(board, scoreStatus);
		}
		break;
	case '8':
		ShellExecute(0, 0, L"https://i.imgur.com/0u47UC2.jpg", 0, 0, SW_SHOW);
		break;
	case '9':
		printGameOver();
		return END_GAME;
	}
}

void TetrisGame::continueBlink() {
	setTextColor(DARKGREY);
	gotoxy(16, 8);
	cout << "(2)";
	gotoxy(28, 8);
	cout << "CONTINUE";
	Sleep(200);

	setTextColor(WHITE);
	gotoxy(16, 8);
	cout << "(2)";
	gotoxy(28, 8);
	cout << "CONTINUE";
	Sleep(200);
}

void TetrisGame::setGameStarted() {

	setTextColor(DARKGREY);
	gotoxy(16, 6);
	cout << "(1) START GAME";
	if (gameStarted) {
		setTextColor(LIGHTGREY);
		gotoxy(16, 8);
		cout << "(2)";
		gotoxy(28, 8);
		cout << "CONTINUE";
		return;
	}
	gameStarted = 1;
}

void TetrisGame::initGame(){

	TetrisBoard board;
	Score scoreStatus;
	setKeys();
	printMenu();
	displayBorder();
	scoreStatus.printScore();
	scoreStatus.printSpeed();
	scoreStatus.printParts();
	currentShape.createShape(rand() % 2 + 10);

	while (true) // the game runs as long as the user doesn't press 9 or the game ends
	{
		char keyPressed;
		
		if (gameStarted)
			continueBlink();

		while (_kbhit())
		{
			keyPressed = _getch();
			if (MenuControl(keyPressed, board, scoreStatus) == END_GAME)
				return;
		}
	}

	
}

void TetrisGame::printMenu(){

	// STATUS BAR
	gotoxy(0, 0);
	cout << "Parts:     " << "Speed:         " << "  Score:  " << endl;
	for (int k = 0; k < 39; k++)
		cout << (unsigned char)175;

	// START - MENU BORDERS
	gotoxy(14,2);
	cout << (unsigned char)201;
	for (int k = 0; k < 23; k++)
		cout << (unsigned char)205;
	cout << (unsigned char)187 << endl;

	for (int j = 2; j < 17; j++){
		gotoxy(14, j+1);
		cout << (unsigned char)186;
		gotoxy(38, j + 1);
		cout << (unsigned char)186 << endl;
	}
	gotoxy(14, 18);
	cout << (unsigned char)200;
	for (int k = 0; k < 23; k++)
		cout << (unsigned char)205;
	cout << (unsigned char)188 << endl;
	// END MENU BORDERS

	// HEDLINE MENU
	gotoxy(23,3);
	cout << "M E N U" << endl;
	gotoxy(15, 4);
	for (int k = 0; k < 23; k++)
		cout << (unsigned char)175;
	
	gotoxy(16,6);
	cout << "(1) START GAME" ;
	gotoxy(16, 8);
	cout << "(2) PAUSE / CONTINUE" ;
	gotoxy(16, 10);
	cout << "(3) SPEED UP" ;
	gotoxy(16, 12);
	cout << "(4) SPEED DOWN";
	gotoxy(16, 14);
	cout << "(8) HELP!" ;
	gotoxy(16, 16);
	cout << "(9) EXIT";
}

void TetrisGame::displayBorder(){
	
	gotoxy(0, 3);

	for (int j = 2; j < 17; j++){
		cout << (unsigned char)186;
		gotoxy(11, j + 1);
		cout << (unsigned char)186 << endl;
	}

	cout << (unsigned char)200;
	for (int k = 0; k < 10; k++)
		cout << (unsigned char)205;
	cout << (unsigned char)188 << endl;

	cout << endl;
}

int TetrisGame::checkKeys(char ch){
	for (int i = 0; i < 11; i++){
		if (ch == keyboards[i])
			return i;
	}
	return invalid_Key;
}

int TetrisGame::runGame(TetrisBoard& board, Score& scoreStatus) {
	int maxY, minY, isBombed = 1 , speed;
	char keyEntered;
	unsigned long int validKey, currentTime, whichShape = currentShape.getShape(); // update 
	int checkPosition, timeInterval , howManyBombed = 0;
	updateInterval(timeInterval, scoreStatus);

	while (true)
	{
		currentTime = (unsigned long int) GetTickCount64() + timeInterval; // the shape goes down every timeInerval ms

		while (GetTickCount64() <= currentTime) { 
			if (_kbhit()) { // otherwise, check for an input
				keyEntered = _getch();
				validKey = checkKeys(keyEntered); // checks if the pressed key is valid
				
				if (checkExit(keyEntered)) return END_GAME;
				if (checkPause(keyEntered)) return PAUSED;

				if (validKey != invalid_Key) {
					speed = scoreStatus.getSpeed();

					//increases or decreases the current speed if possible
					if ((keyEntered == THREE && speed < Score::VERY_HIGH) || (keyEntered == FOUR && speed > Score::VERY_SLOW)) {
						changeSpeed(keyEntered, timeInterval, scoreStatus);
						break;
					}


					// checks if there's a free space in the direction entered OR if it's a joker it'll also return true
					if (board.checkPos(currentShape, validKey) == TetrisBoard::FREE_SPACE) 
					{ 
						// Space key has been pressed - hard drop
						if (keyEntered == SPACE_key) 
							hardDrop(scoreStatus, timeInterval, currentTime, minY, maxY);
						
						//Stop the Joker
						else if (currentShape.getShape() == Shape::JOKER && (keyEntered == s_key || keyEntered == S_key))
						{
								board.updateBoard(currentShape);
								isBombed = 0;
								break;
						}

						//Down key has been pressed - soft drop
						else if (validKey == Shape::DOWN) {
							scoreStatus.updateScoreValue(1); // increases score by 1
							scoreStatus.printScore();
							setTextColor(currentShape.whichColor());
						}
							currentShape.move(validKey, board);

					}

					//check if the bomb needs to explode on the direction entered
					else if (currentShape.getShape() == Shape::BOMB && (keyEntered == LEFT_KEY || keyEntered == RIGHT_KEY) && currentShape.shape[0].getX() > 1 && currentShape.shape[0].getX() < 10) { // the bomb has exploded						currentShape.checkBomb(validKey, board, howManyBombed);
						if (currentShape.checkBomb(validKey, board, howManyBombed)) {
							isBombed = 0; // the bomb has been detonated
							break;
						}
					}


				}
			}
		}

		 checkPosition = board.checkPos(currentShape, Shape::DOWN);

		 //additional tests for the shapes, if it can move/detonate (the bomb).
		 if ((checkPosition == TetrisBoard::FREE_SPACE 
			 || (currentShape.getShape() == Shape::BOMB && !(currentShape.checkBomb(Shape::DOWN, board, howManyBombed)) && currentShape.shape[0].getY() > 18) 
			 || (currentShape.getShape() == Shape::JOKER && checkPosition == TetrisBoard::SHAPE_ENCOUNTER)) && isBombed)
			 currentShape.move(Shape::DOWN, board);

		 //start a new round (creates new shapes and updates the board with the old one)
		 else 
			 newRound(isBombed,timeInterval, board, minY, maxY, scoreStatus, howManyBombed, whichShape);
		 

		 if (board.checkEndGame()) {
				printGameOver();
				return END_GAME;
			}


	}
}

void TetrisGame::newRound(int& isBombed, int& timeInterval, TetrisBoard& board, int& minY, int& maxY, Score& scoreStatus, int& howManyBombed, unsigned long int& whichShape) {
	isBombed = 1;
	updateInterval(timeInterval, scoreStatus);

	if (currentShape.getShape() != Shape::BOMB)
		board.updateBoard(currentShape);

	currentShape.getMinMaxShape(minY, maxY);
	scoreStatus.setLinesDeleted(board.deleteLines(currentShape, minY, maxY),currentShape);
	scoreStatus.updateScoreValue(scoreStatus.getLinesDeleted());
	scoreStatus.updateScoreValue(-50 * howManyBombed);
	howManyBombed = 0;
	scoreStatus.printParts();
	scoreStatus.printScore();
	whichShape = randomNum();
	currentShape.deleteShape();
	currentShape.createShape(whichShape); // creates a new shape
	currentShape.move(Shape::DOWN, board);
}
