#include "TetrisGame.h"
#include "GoToXY.h"
#include "Config.h"
#include "TetrisBoard.h"
#include <thread>
#include <chrono>

#define invalid_Key -1

void TetrisGame::initGame(){
	displayBorder();
	printMenu();
	runGame();
}

void TetrisGame::printMenu(){
	gotoxy(14,3);
	

	for (int j = 2; j < 17; j++){
		gotoxy(14, j+1);
		cout << "*";
		gotoxy(34, j + 1);
		cout << "*" << endl;
	}
	for (int k = 0; k < 12; k++)
		cout << "-";

	cout << endl;
}


void TetrisGame::displayBorder(){
	cout << "Speed:" << " Parts: " << endl;
	cout << "Score: ";
	gotoxy(0, 3);

	

	for (int j = 2; j < 17; j++){
		cout << "#";
		gotoxy(11, j + 1);
		cout << "#" << endl;
	}
	for (int k = 0; k < 12; k++)
		cout << "-";

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
	unsigned long int validKey, currentTime, whichShape = rand() % 2 + 10; // update 
	char keyEntered = '5';
	int checkPosition, timeInterval = 800;
	setKeys();
	board.setBoard();
	currentShape.createShape(whichShape);
	while (keyEntered != ESC)
	{

		currentTime = GetTickCount64() + timeInterval;
		while (GetTickCount64() <= currentTime){
			if (_kbhit()){
				keyEntered = _getch();
				validKey = checkKeys(keyEntered);
				if (validKey != invalid_Key && validKey != ESC){

					if (keyEntered == THREE) //accelerate speed
					{
						currentTime -= 800;
						tempTime -= 200;
						timeInterval += tempTime;

						break;
					}
					else if (keyEntered == FOUR) // decrease speed
					{
						currentTime -= 800;
						tempTime += 200;
						timeInterval += tempTime;
						break;

					} 

					if (board.checkPos(currentShape, validKey) == TetrisBoard::FREE_SPACE){ // returns true also if it's a joker
						if (keyEntered == SPACE_key){
							currentTime -= 800;
				 			timeInterval = 0;
						}
						else if (currentShape.getShape() == Shape::JOKER && (keyEntered == s_key || keyEntered == S_key))
						{
							board.updateBoard(currentShape);
							isBombed = 0;
							break;
						}
						currentShape.move(validKey, board);
					}
					else if (currentShape.getShape() == Shape::BOMB){
						currentShape.checkBomb(validKey, board);
						isBombed = 0;
						break;
					}
     
				}
			}
		}
		checkPosition = board.checkPos(currentShape, Shape::DOWN);
		if ((checkPosition == TetrisBoard::FREE_SPACE || (currentShape.getShape() == Shape::BOMB && currentShape.checkBomb(Shape::DOWN, board) && currentShape.shape[0].getY()>18) ||
			(currentShape.getShape() == Shape::JOKER && checkPosition == TetrisBoard::SHAPE_ENCOUNTER)) && isBombed)
			currentShape.move(Shape::DOWN, board);
		else{ // the last object stopped and a new one needs to be created
			isBombed = 1;
			timeInterval = 800 + tempTime;
			if (currentShape.getShape() != Shape::BOMB)
				board.updateBoard(currentShape);
			currentShape.getMinMaxShape(minY, maxY);
			board.deleteLines(currentShape, minY, maxY);
			whichShape = randomNum();

			

			currentShape.deleteShape();
			currentShape.createShape(whichShape); // creates a new shape
			currentShape.move(Shape::DOWN,board);
		}

		if (board.checkEndGame()){
			gotoxy(1, 20);
			cout << "Game over";                                          
			break;
		}


	}
	gotoxy(2, 17);
	keyEntered = _getch();
	exit(0);
}

