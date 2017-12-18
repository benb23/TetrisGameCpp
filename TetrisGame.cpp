#include "TetrisGame.h"
#include "ScoreBar.h"
#include "GoToXY.h"
#include "Config.h"
#include "TetrisBoard.h"
#include <thread>
#include <chrono>

#define invalid_Key -1

void TetrisGame::initGame(){
	
	displayBorder();
	printMenu();

	while (true) 
	{
		if (_getch() == '1') // the game starts only when the key 1 has been pressed	
		{
			runGame();
			break;
		}
		else if (_getch() == '9') // exit the game in the beggining
			break;
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

void TetrisGame::runGame(){
	int maxY, minY, isBombed = 1, tempTime=0;
	TetrisBoard board;
	Score scoreStatus;
	unsigned long int validKey, currentTime, whichShape = rand() % 2 + 10; // update 
	char keyEntered = '5';
	int checkPosition, timeInterval = 800, howManyBombed=0;

	setKeys();
	scoreStatus.printScore();
	scoreStatus.printSpeed();
	board.setBoard();
	currentShape.createShape(whichShape);

	while (keyEntered != ESC)
	{

		currentTime = GetTickCount64() + timeInterval;
		while (GetTickCount64() <= currentTime){
			if (_kbhit()){
				keyEntered = _getch();
				validKey = checkKeys(keyEntered);
				if (keyEntered == '9'){
					setTextColor(WHITE);
					gotoxy(15, 20);
					cout << "Goodbye! " << endl << endl;
					Sleep(1000);
					return;
				}
				if (validKey != invalid_Key){

					if (keyEntered == THREE) //accelerate speed
					{
						scoreStatus.increaseSpeed();
						scoreStatus.printSpeed();
						currentTime -= 800;
						tempTime -= 200;
						timeInterval += tempTime;

						break;
					}
					else if (keyEntered == FOUR) // decrease speed
					{
						scoreStatus.decreaseSpeed();
						scoreStatus.printSpeed();
						currentTime -= 800;
						tempTime += 200;
						timeInterval += tempTime;
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
			gotoxy(15, 20);
			cout << "Game over" << endl << endl;
			Sleep(1000);
			return;
		}


	}
	gotoxy(2, 17);
	keyEntered = _getch();
	return;
}

