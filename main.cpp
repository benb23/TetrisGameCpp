#include "Config.h"
#include "TetrisGame.h"
#include <time.h>
#include <stdlib.h>

void main(){
	srand((unsigned int)time(0));
	TetrisGame().initGame();
	system("cls");
}