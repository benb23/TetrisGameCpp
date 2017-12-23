#ifndef _Go_To_XY
#define _Go_To_XY
#include "Config.h"

void gotoxy(int x, int y);		// Move the cursor to position according an input coordinate
void clrscr();					// Clear the console screen
void setTextColor(Color);		// Update the console text color
void hideCursor();				// Hides the cursor

#endif