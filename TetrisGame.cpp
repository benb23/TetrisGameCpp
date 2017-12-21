#include "TetrisGame.h"
#include "ScoreBar.h"
#include "GoToXY.h"
#include "Config.h"
#include "TetrisBoard.h"
#include <thread>
#include <chrono>

#define invalid_Key -1

void TetrisGame:: MenuControl(char keyPressed, TetrisBoard& board, Score& scoreStatus) {

	switch (keyPressed) {
	
	case '1':
		setGameStarted();
		runGame(board, scoreStatus);
		break;

	case '2':
		if (gameStarted) {
			setGameStarted(); //prints the normal continue color
			runGame(board, scoreStatus);
		}
		break;
	case '8':
		ShellExecute(0, 0, L"http://www.tetrisfriends.com/help/tips_beginner.php", 0, 0, SW_SHOW);
		break;
	case '9':
		printGameOver();
		return;
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
	board.setBoard();
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
			MenuControl(keyPressed, board, scoreStatus);
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
	cout << "(8) HELP - KEYS" ;
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

void TetrisGame::runGame(TetrisBoard& board, Score& scoreStatus) {
	int maxY, minY, isBombed = 1, tempTime = 0, speed;
	char keyEntered = '5';
	unsigned long int validKey, currentTime, whichShape = currentShape.getShape(); // update 
	int checkPosition, timeInterval = 800, howManyBombed = 0;

	while (true)
	{

		currentTime = GetTickCount64() + timeInterval;
		while (GetTickCount64() <= currentTime) {
			if (_kbhit()) {
				keyEntered = _getch();
				validKey = checkKeys(keyEntered);
				if (checkExit(keyEntered) || checkPause(keyEntered))
					return;
				if (validKey != invalid_Key) {
					speed = scoreStatus.getSpeed();
					if ((keyEntered == THREE && speed < Score::VERY_HIGH) || (keyEntered == FOUR && speed > Score::VERY_SLOW)) {
						changeSpeed(keyEntered, tempTime, timeInterval, scoreStatus);
						break;
					}

					if (board.checkPos(currentShape, validKey) == TetrisBoard::FREE_SPACE) { // returns true also if it's a joker
						if (keyEntered == SPACE_key) // space key has been pressed
						{
							currentShape.getMinMaxShape(minY, maxY);
							scoreStatus.setDistance(currentShape, minY);
							scoreStatus.updateScoreValue(2 * scoreStatus.getDistance()); // hard drop 
							currentTime -= 800;
							timeInterval = 0;
							}
							else if (currentShape.getShape() == Shape::JOKER && (keyEntered == s_key || keyEntered == S_key))
							{
								board.updateBoard(currentShape);
								isBombed = 0;
								break;
							}
							else if (validKey == Shape::DOWN) {
								scoreStatus.updateScoreValue(1); // increases score by 1
								scoreStatus.printScore();
								setTextColor(currentShape.whichColor());
								flushall();

							}
							currentShape.move(validKey, board);

						}
						else if (currentShape.getShape() == Shape::BOMB && (keyEntered == LEFT_KEY || keyEntered == RIGHT_KEY) && currentShape.shape[0].getX() > 1 && currentShape.shape[0].getX() < 10) { // the bomb has exploded						currentShape.checkBomb(validKey, board, howManyBombed);
							if (currentShape.checkBomb(validKey, board, howManyBombed)) {
								isBombed = 0;
								break;
							}
						}


					}
				}
			}
			checkPosition = board.checkPos(currentShape, Shape::DOWN);
			if ((checkPosition == TetrisBoard::FREE_SPACE || (currentShape.getShape() == Shape::BOMB && !(currentShape.checkBomb(Shape::DOWN, board, howManyBombed)) && currentShape.shape[0].getY() > 18) ||
				(currentShape.getShape() == Shape::JOKER && checkPosition == TetrisBoard::SHAPE_ENCOUNTER)) && isBombed)
				currentShape.move(Shape::DOWN, board);
			else { // the last object stopped and a new one needs to be created
				newRound(isBombed, tempTime, timeInterval, board, minY, maxY, scoreStatus, howManyBombed, whichShape);
			}

			if (board.checkEndGame()) {
				printGameOver();
				return;
			}


		}
		gotoxy(2, 17);
		keyEntered = _getch();
		return;

}

void TetrisGame::newRound(int& isBombed,int& tempTime, int& timeInterval, TetrisBoard& board, int& minY, int& maxY, Score& scoreStatus, int& howManyBombed, unsigned long int& whichShape) {
	isBombed = 1;
	timeInterval = 800 + tempTime;
	if (currentShape.getShape() != Shape::BOMB)
		board.updateBoard(currentShape);
	currentShape.getMinMaxShape(minY, maxY);

	scoreStatus.setLinesDeleted(board.deleteLines(currentShape, minY, maxY));
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
