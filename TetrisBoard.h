#ifndef _Board_H
#define _Board_H

#include "Shape.h"
#include "Config.h"

class TetrisBoard{
private:
	int Board[ROWS][COLUMNS];
public:
	enum { BOTTOM_ENCOUNTER = 11, SHAPE_ENCOUNTER = 12, FREE_SPACE };
	
	void printBoard(int currentY){
		for (int y = currentY; y > Board_Gap; y--){
			for (int x = 1; x <= COLUMNS; x++){
				gotoxy(x, y);
				switch (getCoord(x, y)){
					
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

	void setBoard(){
		for (int i = 0; i < ROWS; i++){
			for (int j = 0; j < COLUMNS; j++){
				Board[i][j] = 0;
			}
		}
	}


	void updateBoard(Shape current){
		int x, y;
		for (int i = 0; i < current.SIZE; i++){
			x = current.shape[i].getX();
			y = current.shape[i].getY();
			Board[y-Board_Gap][x-1]=current.getShape();
		}
	}

	void setCoord(int _x, int _y, int val){
		Board[_y - Board_Gap][_x - 1] = val;
	}
	
	int getCoord(int _x, int _y){
		if (_y <= 2 || _x>=19)
			return 0;
		else
			return Board[_y - Board_Gap][_x - 1];
	}

	bool checkEndGame(){
		for (int i = 0; i < 15; i++){
			if (Board[0][i] != 0)
				return TRUE;
		}
		return FALSE;
	}

	int checkPos(Shape current, int direction);

	bool checkBoard(int _x, int _y){
		 if (getCoord(_x,_y))
			 return TRUE;
		 else
			 return FALSE;
		
	}

	bool checkLine(int currentY);

	int deleteLines(Shape current, int minY, int maxY);

};

#endif _Board_H