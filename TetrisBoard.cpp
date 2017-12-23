#include "TetrisBoard.h"
#include "GoToXY.h"

int TetrisBoard::checkPos(const Shape& current, int direction){
	int x, y;
	switch (direction){
	case Shape::LEFT:

		for (int i = 0; i < current.SIZE;i++) {
			x = current.shape[i].getX() - 1;
			y = current.shape[i].getY();
			if (x == 0)
				return BOTTOM_ENCOUNTER;
			else if(Board[y - Board_Gap][x - 1] != 0 && current.getShape()!=Shape::JOKER)
				return SHAPE_ENCOUNTER;

		}
		break;
	case Shape::RIGHT:
		for (int i = 0; i < current.SIZE;i++) {
			x = current.shape[i].getX() + 1;
			y = current.shape[i].getY();
			if (x == 11)
				return BOTTOM_ENCOUNTER;
			else if (Board[y - Board_Gap][x - 1] != 0 && current.getShape() != Shape::JOKER)
				return SHAPE_ENCOUNTER;
		}
		break;
	case Shape::DOWN:
		for (int i = 0; i < current.SIZE;i++) {
			x = current.shape[i].getX();
			y = current.shape[i].getY() + 1;
			if (y == 18)
				return BOTTOM_ENCOUNTER;
			else if(Board[y - Board_Gap][x - 1] != 0)
				return SHAPE_ENCOUNTER;
		}
		break;
	}
	return FREE_SPACE;
}

bool TetrisBoard::checkLine(int currentY){

	for (int x = 1; x <= COLUMNS; x++){
		if (!checkBoard(x, currentY))
			return false;
	}
	return true;

}

int TetrisBoard::deleteLines(const Shape& current, int minY, int maxY){


	int currentY = minY, howManyDel = 0, temp;

	while (currentY - maxY >= 0){

		
		if (checkLine(currentY)) // need to delete the relatively lowest line for the shape
		{ 
			gotoxy(1, currentY);
			for (int i = 1; i <= COLUMNS; i++){
				setCoord(i, currentY, 0);
				cout << " ";
			}

			for (int y = currentY; y > Board_Gap; y--) // updating the board
			{
				for (int x = 1; x <= COLUMNS; x++){
					temp = getCoord(x, y-1);
					setCoord(x, y, temp);
				}
			}
			Sleep(250);
			printBoard(currentY);
			howManyDel++;
			maxY++;

		}
		else // check lines above the relatively lowest line
			currentY--;
	}

	return howManyDel;

}

void TetrisBoard::setBoard() {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLUMNS; j++) {
			Board[i][j] = 0;
		}
	}
}

bool TetrisBoard::checkEndGame() {
	for (int i = 0; i < 15; i++) {
		if (Board[0][i] != 0)
			return TRUE;
	}
	return FALSE;
}

void TetrisBoard::updateBoard(Shape current) {
	int x, y;
	for (int i = 0; i < current.SIZE; i++) {
		x = current.shape[i].getX();
		y = current.shape[i].getY();
		Board[y - Board_Gap][x - 1] = current.getShape();
	}
}

void TetrisBoard::printBoard(int currentY) {
	for (int y = currentY; y > Board_Gap; y--) {
		for (int x = 1; x <= COLUMNS; x++) {
			gotoxy(x, y);
			switch (getCoord(x, y)) {

			case Shape::LINE:
				setTextColor(LIGHTCYAN);
				cout << "%";
				break;
			case Shape::CUBE:
				setTextColor(LIGHTMAGENTA);
				cout << "%";
				break;
			case Shape::JOKER:
				setTextColor(YELLOW);
				cout << "X";
				break;
			default:
				cout << " ";
				break;

			}
		}
	}
}