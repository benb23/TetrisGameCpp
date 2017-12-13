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

void Shape::move(int direction, TetrisBoard& board) {
	int position = getPoisition();
	int x,y;

	for (int j = 0; j < SIZE; j++){
		// if the shape is the Joker AND there is another shape there
		
		if (getShape() == JOKER && board.getCoord(shape[0].getX(),shape[0].getY())){
 			setTextColor(whichColor(board.getCoord(shape[j].getX(), shape[j].getY())));
			if (board.getCoord(shape[j].getX(), shape[j].getY()) == JOKER)
				shape[j].draw('X');
			 else
				shape[j].draw('%');


		} else 
			shape[j].draw(' ');
	}
		
	

	if (direction != UP){
		for (int j = 0; j < SIZE; j++)
			shape[j].move(direction);
	}
	else {
		
		if (currentShape == LINE){
			if (position == DEG_0 || position == DEG_180) //faces down
			{
				for (int j = 0; j < SIZE; j++){
					if (shape[j].getY() >= 16 || board.checkBoard(shape[j].getX(), shape[j].getY() + 1)
						|| board.checkBoard(shape[j].getX(), shape[j].getY() + 2))
					{
						for (int i = 0; i < SIZE; i++){
							y = shape[i].getY();
							shape[i].setY(y - 2);
						}
						break;
					}
				}
			}
			else // checking right/left 
			{
				for (int j = 0; j < SIZE; j++){
					if (shape[j].getX() <= 1 ||  board.checkBoard(shape[j].getX()-1, shape[j].getY())
						|| board.checkBoard(shape[j].getX() - 2, shape[j].getY() ))
					{
						for (int i = 0; i < SIZE; i++){
							x = shape[i].getX();
							if (position == DEG_90)
								shape[i].setX(x + 2);
							else
								shape[i].setX(x + 1);
						}
						break;
					}
					if (shape[j].getX() >= 9 || board.checkBoard(shape[j].getX() + 1, shape[j].getY())
						|| board.checkBoard(shape[j].getX() + 2, shape[j].getY()))
					{
						for (int i = 0; i < SIZE; i++){
							x = shape[i].getX();
							if (position==DEG_90)
								shape[i].setX(x - 1);
							else
								shape[i].setX(x - 2);
						}
						break;
					}
				}
			}
		}
		rotate(position);
	}
	setTextColor(whichColor());
	for (int j = 0; j < SIZE; j++)
		shape[j].draw(getTexture());
}

void Shape::rotate(int position) {

	int x, y, k;
	switch (position) {

	case DEG_0:

		switch (getShape())
		{

		case LINE:
			k = -1;
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
			k = -2;
			for (int i = 0; i < SIZE; i++) {
				x = shape[i].getX();
				y = shape[i].getY();
				shape[i].setX(x + k);
				shape[i].setY(y - k);
				k++;
			}
			setPosition(DEG_180);
		}
		break;

	case DEG_180:

		switch (getShape())
		{
		case LINE:
			k = -2;
			for (int i = 0; i < SIZE;i++) {
				x = shape[i].getX();
				y = shape[i].getY();
				if (i != 2)
					shape[i].draw(' ');
				shape[i].setX(x - k - 1);
				shape[i].setY(y + k);
				k++;
			}
			setPosition(DEG_270);
		}
		break;

	case DEG_270:

		switch (getShape())
		{
		case LINE:
			k = -1;
			for (int i = 0; i < SIZE;i++) {
				x = shape[i].getX();
				y = shape[i].getY();
				if (i != 1)
					shape[i].draw(' ');
				shape[i].setX(x+k);
				shape[i].setY(y-k);

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

	if ((direction == LEFT && board.checkBoard(x - 1, y) == true) ||
		(direction == RIGHT && board.checkBoard(x + 1, y) == true) ||
		(board.checkBoard(x, y + 1) == true))
	{
		howManyBombed = activateBomb(x, y, board);
		return false;
	}


	gotoxy(x, y);
	return true;


}

int Shape::activateBomb(int x, int y, TetrisBoard& board){
	int tempX, tempY = y - 1 , howManyBombed = 0;
	for (int i = 0; i < 3; i++){

		tempX = x - 1;
		for (int j = 0; j < 3; j++){

			if (tempX > 0 && tempX <= 10 && tempY > 3 && tempY < 18){
				if (board.getCoord(tempX, tempY))
					howManyBombed++;
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


