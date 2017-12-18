#include "TetrisGame.h"
#include "ScoreBar.h"
#include "GoToXY.h"
#include "Config.h"
#include "TetrisBoard.h"
#include <thread>
#include <chrono>

#define invalid_Key -1

void TetrisGame::initGame(){

	TetrisBoard board;
	Score scoreStatus;
	setKeys();
	board.setBoard();
	printMenu();
	displayBorder();
	scoreStatus.printScore();
	scoreStatus.printSpeed();
	currentShape.createShape(rand() % 2 + 10);



	while (true) 
	{
		char keyPressed;

		while (_kbhit) {
			keyPressed = _getch();

			switch (keyPressed) {
			case '1':
				runGame(board,scoreStatus);
				break;

			case '2':
				runGame(board, scoreStatus);
				break;
			case '8':
				ShellExecute(0, 0, L"http://www.tetrisfriends.com/help/tips_beginner.php", 0, 0, SW_SHOW);
				break;
			case '9':
				setTextColor(LIGHTGREEN);
				gotoxy(0, 20);
				cout << "     _____                         ____                 " << endl;
				cout << "    / ____|                       / __ \\                " << endl;
				cout << "   | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ " << endl;
				cout << "   | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|" << endl;
				cout << "   | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   " << endl;
				cout << "    \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   " << endl;
				Sleep(5000);
				return;
			}
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
	gotoxy(21,3);
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
	cout << "(8) HELP" ;
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

void TetrisGame::runGame(TetrisBoard& board, Score& scoreStatus){
	int maxY, minY, isBombed = 1, tempTime = 0;
	char keyEntered = '5';
	unsigned long int validKey, currentTime, whichShape=currentShape.getShape(); // update 
	int checkPosition, timeInterval = 800, howManyBombed = 0;

	while (keyEntered != ESC)
	{

		currentTime = GetTickCount64() + timeInterval;
		while (GetTickCount64() <= currentTime){
			if (_kbhit()){
				keyEntered = _getch();
				validKey = checkKeys(keyEntered);
				if (checkExit(keyEntered) || checkPause(keyEntered))
					return;
				if (validKey != invalid_Key){

					if (keyEntered == THREE) //accelerate speed
					{
						if (scoreStatus.getSpeed() < Score::VERY_HIGH) {
							tempTime -= 300;
							timeInterval = 800 + tempTime;
							scoreStatus.increaseSpeed();
							scoreStatus.printSpeed();
						}
						break;
					}
					else if (keyEntered == FOUR) // decrease speed
					{
						if (scoreStatus.getSpeed() > Score::VERY_SLOW) {
							tempTime += 300;
							timeInterval = 800 + tempTime ;
							scoreStatus.decreaseSpeed();
							scoreStatus.printSpeed();
						}
						break;

					} 

					if (board.checkPos(currentShape, validKey) == TetrisBoard::FREE_SPACE){ // returns true also if it's a joker
						if (keyEntered == SPACE_key) // space key has been pressed
						{
							currentShape.getMinMaxShape(minY, maxY);
							scoreStatus.setDistance(currentShape,minY);
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
						else if (validKey == Shape::DOWN){
							scoreStatus.updateScoreValue(1); // increases score by 1
							scoreStatus.printScore();
							setTextColor(currentShape.whichColor());
							flushall();

						}
						currentShape.move(validKey, board);

					}
					else if (currentShape.getShape() == Shape::BOMB && (keyEntered==LEFT_KEY || keyEntered == RIGHT_KEY) && currentShape.shape[0].getX() >  1 && currentShape.shape[0].getX() < 10){ // the bomb has exploded						currentShape.checkBomb(validKey, board, howManyBombed);
						if (currentShape.checkBomb(validKey, board, howManyBombed)) {
							isBombed = 0;
							break;
						}
					}

     
				}
			}
		}
		checkPosition = board.checkPos(currentShape, Shape::DOWN);
		if ((checkPosition == TetrisBoard::FREE_SPACE || (currentShape.getShape() == Shape::BOMB && !(currentShape.checkBomb(Shape::DOWN, board, howManyBombed)) && currentShape.shape[0].getY()>18) ||
			(currentShape.getShape() == Shape::JOKER && checkPosition == TetrisBoard::SHAPE_ENCOUNTER)) && isBombed)
			currentShape.move(Shape::DOWN, board);
		else{ // the last object stopped and a new one needs to be created
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
			currentShape.move(Shape::DOWN,board);
		}

		if (board.checkEndGame()){
			setTextColor(WHITE);
			gotoxy(0, 20);
			cout << "     _____                         ____                 " << endl;
			cout << "    / ____|                       / __ \\                " << endl;
			cout << "   | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __ " << endl;
			cout << "   | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|" << endl;
			cout << "   | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |   " << endl;
			cout << "    \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|   " << endl;
			Sleep(5000);
			return;
		}


	}
	gotoxy(2, 17);
	keyEntered = _getch();
	return;
}

