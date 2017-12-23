
#include "GoToXY.h"
#include "TetrisBoard.h"


void Shape::createShape(int whichShape) {
	int k=1;
	switch (whichShape) {
	case CUBE:
		shape = new Point[4];

		for (int i = 0; i < 4; i++) {
			if (i % 2 == 0) {
				shape[i].setX(5);
				shape[i].setY(k+2);
			}
			else {
				shape[i].setX(6);
				shape[i].setY(k+2);
				k--;
			}
		}
		setTexture('%');
		SIZE = 4;
		break;

	case LINE:
		shape = new Point[4];
		for (int i = 0; i < 4; i++) {
			shape[i].setX(4 + i);
			shape[i].setY(2);
		}
		SIZE = 4;
		setTexture('%');
		break;

	case JOKER:
		shape = new Point;
		shape->setX(6);
		shape->setY(2);
		setTexture('X');
		SIZE = 1;
		break;
	case BOMB:
		shape = new Point;
		shape->setX(6);
		shape->setY(2);
		setTexture('@');
		SIZE = 1;
		break;
	}
	setTextColor(whichColor(whichShape));
	setShape(whichShape);
	setPosition(DEG_0);
}

bool Shape::canTheShapeRotate(const TetrisBoard& board) {
	int x, y;
	for (int i = 0; i < SIZE; i++) //checks if the current shape has reached the bottom of the screen or near another shape
	{
		x = shape[i].getX();
		y = shape[i].getY();
		if (y >= Board_Gap + ROWS - 1 || board.checkBoard(x, y + 1)
			|| (board.checkBoard(x + 1, y) && board.checkBoard(x - 1, y)) // close to shapes in both sides left+right
			|| (board.checkBoard(x + 2, y) && board.checkBoard(x - 1, y))
			|| (board.checkBoard(x + 1, y) && board.checkBoard(x - 2, y))
			|| (x >= COLUMNS - 2 && board.checkBoard(x - 1, y)) // close to a wall from the right + a shape in the left
			|| (x <= 1 && board.checkBoard(x + 1, y))) // close to a wall in the left + a shape in the right
			return false;
	}
	return true;
}


void Shape::move(int direction,const TetrisBoard& board) {
	int position = getPoisition();
	int x, y, check = 1;

	for (int j = 0; j < SIZE; j++){

		// if the shape is the Joker AND there is another shape there then print the joker in top of it 
		if (getShape() == JOKER && board.getCoord(shape[0].getX(),shape[0].getY())){
 			setTextColor(whichColor(board.getCoord(shape[j].getX(), shape[j].getY())));
			if (board.getCoord(shape[j].getX(), shape[j].getY()) == JOKER)
				shape[j].draw('X');
			 else 	// print the previous shape 
				shape[j].draw('%');

		} else 
			shape[j].draw(' ');
	}
		
	

	if (direction != UP){ // the shape won't rotate
		for (int j = 0; j < SIZE; j++)
			shape[j].move(direction);
	}
	else {
		
		if (currentShape == LINE)
		{
			check = canTheShapeRotate(board);

			if (check) {
				if (position == DEG_0 || position == DEG_180) //faces down
				{
					for (int j = 0; j < SIZE; j++) {
						if (shape[j].getY() == 16 || board.checkBoard(shape[j].getX(), shape[j].getY() + 1)
							|| board.checkBoard(shape[j].getX(), shape[j].getY() + 2))
						{
							for (int i = 0; i < SIZE; i++) {
								y = shape[i].getY();
								shape[i].setY(y - 2);
							}
							break;
						}
					}
				}
				else // checking right/left 
				{
					for (int j = 0; j < SIZE; j++) {
						if (shape[j].getX() <= 2 || board.checkBoard(shape[j].getX() - 1, shape[j].getY())
							|| board.checkBoard(shape[j].getX() - 2, shape[j].getY()))
						{
							for (int i = 0; i < SIZE; i++) {
								x = shape[i].getX();
								if (position == DEG_90 && x<2)
									shape[i].setX(x + 2);
								else
									shape[i].setX(x + 1);
							}
							break;
						}
						if (shape[j].getX() >= 9 || board.checkBoard(shape[j].getX() + 1, shape[j].getY())
							|| board.checkBoard(shape[j].getX() + 2, shape[j].getY()))
						{
							for (int i = 0; i < SIZE; i++) {
								x = shape[i].getX();
								if (position == DEG_90)
									shape[i].setX(x - 1);
								else
									shape[i].setX(x - 2);
							}
							break;
						}
					}
				}
			}

			if (check)
				rotate(position);
			}
	}

	setTextColor(whichColor());
	for (int j = 0; j < SIZE; j++)
		shape[j].draw(getTexture());
}

void Shape::rotate(int position) {

	int x, y, k = -1 ;
	switch (position) {

	case DEG_0:
		// the switch case is future proof
		switch (getShape())
		{

		case LINE:
			for (int i = 0; i < SIZE;i++) {
				x = shape[i].getX();
				y = shape[i].getY();
				shape[i].setX(x - k + 1);
				shape[i].setY(y + k);
				k++;
			}
			setPosition(DEG_90);
		}
		break;

	case DEG_90:

		switch (getShape())
		{

		case LINE:
			for (int i = 0; i < SIZE; i++) {
				x = shape[i].getX();
				y = shape[i].getY();
				shape[i].setX(x - k);
				shape[i].setY(y - k + 1);
				k++;
			}
			setPosition(DEG_180);
		}
		break;

	case DEG_180:

		switch (getShape())
		{
		case LINE:
			for (int i = 0; i < SIZE;i++) {
				x = shape[i].getX();
				y = shape[i].getY();
				shape[i].setX(x + k - 1);
				shape[i].setY(y - k);
				k++;
			}
			setPosition(DEG_270);
		}
		break;

	case DEG_270:

		switch (getShape())
		{
		case LINE:
			for (int i = 0; i < SIZE;i++) {
				x = shape[i].getX();
				y = shape[i].getY();
				shape[i].setX(x + k);
				shape[i].setY(y+ k - 1);
				k++;
			}
			setPosition(DEG_0);
		}

		break;
	}
	
}

bool Shape::checkBomb(int direction, TetrisBoard& board, int& howManyBombed){
	int x= shape[0].getX(), y=shape[0].getY();

	shape[0].draw(getTexture());

	// checks if the bomb can explode in the direction entered
	if ((direction == LEFT && board.checkBoard(x - 1, y) == true) ||
		(direction == RIGHT && board.checkBoard(x + 1, y) == true) ||
		(board.checkBoard(x, y + 1) == true) || y>=Board_Gap+ROWS-1)
	{
		howManyBombed = activateBomb(x, y, board); // update the pieces that have been blown
		return true;
	}


	gotoxy(x, y);
	return false;


}

int Shape::activateBomb(int x, int y, TetrisBoard& board){
	int tempX, tempY = y - 1 , howManyBombed = 0;
	for (int i = 0; i < 3; i++){

		tempX = x - 1;
		for (int j = 0; j < 3; j++){
			// if it's within the board limits and a shape has been met, update the counter (howManyBombed)
			if (tempX > 0 && tempX <= COLUMNS && tempY > Board_Gap && tempY < ROWS+Board_Gap){
				if (board.getCoord(tempX, tempY))
					howManyBombed++;

				// explosion effect
				if (j%2==0)
					setTextColor(YELLOW);
				else
					setTextColor(RED);
				gotoxy(tempX, tempY);
				cout << "*";
				Sleep(50);
				
				gotoxy(tempX, tempY);
				cout << " ";
				board.setCoord(tempX, tempY,0);
			}
			tempX++;
		}
		tempY++;

	}
	return howManyBombed;
}

void Shape::getMinMaxShape(int& minY, int& maxY) {
	minY = maxY = shape[0].getY();
	for (int i = 1; i < SIZE; i++) {
		if (shape[i].getY() > minY)
			minY = shape[i].getY();
		if (shape[i].getY()< maxY)
			maxY = shape[i].getY();
	}
}

Color Shape::whichColor(int theShapeNum = 0) {
	
		if (theShapeNum == 0)
			theShapeNum = currentShape;
		switch (theShapeNum) {
		case CUBE:
			return LIGHTMAGENTA;
		case LINE:
			return LIGHTCYAN;
		case JOKER:
			return YELLOW;
		case BOMB:
			return LIGHTRED;
		default:
			return DARKGREY;
		}

}
