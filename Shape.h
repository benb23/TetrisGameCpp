#ifndef _Shape_H
#define _Shape_H

#include "Point.h"
#include "Config.h"
#include "GoToXY.h"
class TetrisBoard;		// Foward decleration for TetrisBoard	

class Shape{
private:
	int currentShape;	// Current shape type
	int position;		// Current
	char texture;		// Shape texture to print 
public:
	Point* shape;

	enum {DEG_0,DEG_90,DEG_180,DEG_270};
	enum { DOWN, LEFT, UP, RIGHT, CUBE = 10, LINE = 11, JOKER = 12, BOMB = 13};
	int SIZE;
	void setPosition(int pos) {
		position = pos;
	}
	int getPoisition() {
		return position;
	}
	int getShape() {
		return currentShape;
	}
	void setShape(int whichShape) {
		currentShape = whichShape;
	}
	void createShape(int whichShape);
	void deleteShape() {
		if (getShape() == BOMB){
			gotoxy(shape[0].getX(), shape[0].getY());
			cout << " ";
		}
		delete[] shape;
	}
	void move(int direction,  TetrisBoard& board);
	void rotate(int position);
	void setTexture(char ch){
		this->texture = ch;
	}

	char getTexture(){
		return texture;
	}

	void getMinMaxShape(int& minY, int& maxY)
	
	{
		minY = maxY = shape[0].getY();
		for (int i = 1; i < SIZE; i++){
			if (shape[i].getY() > minY)
				minY = shape[i].getY();
			if (shape[i].getY()< maxY)
				maxY = shape[i].getY();
		}
	}

	bool checkBomb(int direction, TetrisBoard& board, int& howManyBombed);
	int activateBomb(int x, int y, TetrisBoard& board);

	Color whichColor(int theShapeNum=0){
		if (theShapeNum == 0)
			theShapeNum = currentShape;
		switch (theShapeNum){
			case CUBE:
				return LIGHTMAGENTA;
			case LINE:
				return LIGHTCYAN;
			case JOKER:
				return YELLOW;
			case BOMB:
				return LIGHTRED;
		}
	}

	bool canTheShapeRotate(TetrisBoard& board);
};

#endif _Shape_H