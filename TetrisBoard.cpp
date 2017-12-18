#include "TetrisBoard.h"
#include "GoToXY.h"

int TetrisBoard::checkPos(Shape current, int direction){
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

int TetrisBoard::deleteLines(Shape current, int minY, int maxY){


	int currentY = minY, howManyDel = 0, temp, tempY, val;

	while (currentY - maxY >= 0){

		
		if (checkLine(currentY)) // need to delete the relatively lowest line for the shape
		{ 
			gotoxy(1, currentY);
			for (int i = 1; i <= COLUMNS; i++) // printing the deletion of the line
			{
				setCoord(i, currentY, 0); //updating the deleted row to be ''empty''
				cout << " ";
			}

			for (int y = currentY; y > Board_Gap; y--) // updating the board
			{
				for (int x = 1; x <= COLUMNS; x++){
					val = getCoord(x, y-1);
					tempY = y;
					if (val && current.getShape()!=Shape::JOKER) // if there's a shape above the deleted line 
					{
						while (tempY <= ROWS + Board_Gap - 1 ) {
							temp = getCoord(x, tempY);
							if (temp)
								break;
							setCoord(x, tempY, val);
							if(tempY-1 != y-1) // the shape needs to go down more than 1 row
								setCoord(x, tempY-1, 0);
							tempY++;
						}
						tempY--;
					}
					setCoord(x, tempY, val);
				}
			}
			Sleep(250);
			printBoard(ROWS+Board_Gap-1);
			howManyDel++;
			maxY++;

		}
		else // check lines above the relatively lowest line
			currentY--;



	}

	return howManyDel;

}