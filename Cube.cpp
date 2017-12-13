#include "Cube.h"
#include "Point.h"
#include "GoToXY.h"



void Cube:: setColor(Color c){

}



void Cube::move(int direction) {

	switch (direction){
	case LEFT:
		square[SIZE - 1].draw(' ');
		square[SIZE - 3].draw(' ');
		for (int i = SIZE - 1; i >= 1; i-=2)
			square[i] = square[i - 1];

		square[0].move(direction);
		square[2].move(direction);
		setTextColor(Color::LIGHTMAGENTA);
		square[0].draw('%');
		square[2].draw('%');
		break;
	case RIGHT:
		square[0].draw(' ');
		square[2].draw(' ');
		for (int i = 0; i < SIZE; i+=2)
			square[i] = square[i + 1];

		square[1].move(direction);
		square[3].move(direction);
		setTextColor(Color::LIGHTMAGENTA);
		square[1].draw('%');
		square[3].draw('%');
		break;
	default:
		square[SIZE - 1].draw(' ');
		square[SIZE - 2].draw(' ');
		for (int i = SIZE - 1; i > 1; --i)
			square[i] = square[i - 2];

		square[0].move();
		square[1].move();
		setTextColor(Color::LIGHTMAGENTA);
		square[0].draw('%');
		square[1].draw('%');
		break;
	}




	
}


