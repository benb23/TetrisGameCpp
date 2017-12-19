#include "Point.h"
#include "GoToXY.h"


void Point::draw(char ch) {
	gotoxy(x, y);
	if (ch == ' ')
		setTextColor(BLACK);
	cout << ch;
	hideCursor();
}


void Point::move(int dir = DOWN) {
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