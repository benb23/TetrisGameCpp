#include "TetrisGame.h"
#include "ScoreBar.h"
#include "GoToXY.h"
#include "Config.h"
#include "TetrisBoard.h"
#include <thread>
#include <chrono>


int TetrisGame:: MenuControl(char keyPressed, TetrisBoard& board, Score& scoreStatus) {

	switch (keyPressed) {
	
	case '1':	// start new game button
		if (!gameStarted) {
			setGameStarted();
			return runGame(board, scoreStatus);
		}
		break;
	case '2':	// pause / continue button
		if (gameStarted) {
			setGameStarted(); //prints the normal continue color
			return runGame(board, scoreStatus);
		}
		break;
	case '8': // help button - directing to and url that contain an image 
		ShellExecute(0, 0, L"https://i.imgur.com/0u47UC2.jpg", 0, 0, SW_SHOW);
		break;
	case '9': // EXIT button 
		printGameOver();
		return END_GAME;
	}
	flushall();
}

// The function turning on a blinking effect to the pause/continue 
// button while the game is paused
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

// The function coloring the start game button in grey after clicking 
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

// The function is initializing the game  
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

// The function prints the game borders
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
	int maxY, minY, isBombed = 1, checkPosition, timeInterval, howManyBombed = 0, gameStatus;
	int whichShape = currentShape.getShape(); // update 
	updateInterval(timeInterval, scoreStatus);

	while (true)
	{
		 gameStatus = dropInterval(board, scoreStatus, timeInterval, isBombed, minY, maxY, howManyBombed);
		 if (gameStatus)  return gameStatus; // the game will continue unless the pause or exit button has been pressed

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

void TetrisGame::newRound(int& isBombed, int& timeInterval, TetrisBoard& board, int& minY, int& maxY, Score& scoreStatus, int& howManyBombed,int& whichShape) {
	isBombed = 1;
	updateInterval(timeInterval, scoreStatus);

	if (currentShape.getShape() != Shape::BOMB)
		board.updateBoard(currentShape);

	currentShape.getMinMaxShape(minY, maxY); 
	scoreStatus.setLinesDeleted(board.deleteLines(currentShape, minY, maxY),currentShape); // deletes lines only within the shape's limit
	scoreStatus.updateScoreValue(scoreStatus.getLinesDeleted()); 
	scoreStatus.updateScoreValue(-50 * howManyBombed); // each block the bomb erased costs 50 points
	howManyBombed = 0;
	scoreStatus.printParts();
	scoreStatus.printScore();
	whichShape = randomNum(); 
	currentShape.deleteShape();
	currentShape.createShape(whichShape); // creates a new shape randomly
	currentShape.move(Shape::DOWN, board);
}

int TetrisGame:: dropInterval(TetrisBoard& board, Score& scoreStatus, int& timeInterval, int& isBombed, int&minY, int& maxY, int& howManyBombed)
{
	char keyEntered;
	int validKey, speed;
	unsigned long int currentTime;
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

	return 0;

}
