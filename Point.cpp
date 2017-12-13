#include "Point.h"
#include "GoToXY.h"
void Point::draw(char ch) {
	gotoxy(x, y);
	cout << ch;
	hideCursor();
}

void Point::move() {
	move(0);
}

void Point::move(int dir) {
	switch (dir) {
	case DOWN: 
		++y;
		break;
	case LEFT: 
		--x;
		break;
	case UP: 
		--y;
		break;
	case RIGHT: 
		++x;
		break;
	}
}