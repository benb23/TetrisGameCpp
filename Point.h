#ifndef _Point
#define _Point
#include <iostream>
using namespace std;
#define _SHAPE_STOP -1

class Point{
	enum {DOWN,LEFT,UP,RIGHT};
private:
	int x;
	int y;
public:
	Point(){
		this->x = 7;
		this->y = 0;
	}
	Point(int x, int y){
		this->x = x;
		this->y = y;
	}
	void setPoint(int x, int y){
		this->x = x;
		this->y = y;
	}
	int getX(){
		return this->x;
	}
	int getY(){
		return this->y;
	}
	void setY(int y){
		this->y = y;
	}
	void setX(int x){
		this->x = x;
	}
	void draw(char ch);
	void move(int direction);
};


#endif