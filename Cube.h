#ifndef _Cube
#define _Cube
#include "Point.h"
#include <iostream>
#include "Config.h"

using namespace std;

class Cube{
	
public:
	enum { SIZE = 4, DEFAULT = -999, LEFT = 1, RIGHT = 3 };
	Point square[SIZE];
	int direction;
	void setColor(Color c);
	void move(int direction=DEFAULT);
	
	void setCube(){
		square[0].setX(7);
		square[0].setY(0);
		square[1].setX(8);
		square[1].setY(0);
		square[2].setX(7);
		square[2].setY(0);
		square[3].setX(8);
		square[3].setY(0);
	}

};

#endif