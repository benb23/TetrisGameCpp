#ifndef _ScoreBar_H
#define _ScoreBar_H
#include "TetrisBoard.h"

class Score{
	int scoreValue, distance, linesDeleted, partsNum, speedCounter;
	int maxValueY[COLUMNS];
public:
	enum{VERY_SLOW=-2,SLOW,NORMAL,HIGH,VERY_HIGH};

	int getSpeed(){
		return speedCounter;
	}

	void increaseSpeed(){
		if (speedCounter == VERY_HIGH)
			return;
		speedCounter++; 
	}

	void decreaseSpeed(){
		if (speedCounter == VERY_SLOW)
			return;
		speedCounter--;
	}

	void printSpeed(){
		setTextColor(WHITE);
		gotoxy(18, 0);
		for (int i = 0; i < 9; i++)
			cout << " ";
		gotoxy(18, 0);
		switch (speedCounter){
		case VERY_SLOW:
			cout << "Very Slow";
			break;
		case SLOW:
			cout << "Slow";
			break;
		case NORMAL:
			cout << "Normal";
			break;
		case HIGH:
			cout << "High";
			break;
		case VERY_HIGH:
			cout << "Very High";
			break;
		}
	}

	void setLinesDeleted(int howMany) { 

		switch (howMany){
		case 0:
			linesDeleted = 0;
			break;
		case 1: 
			linesDeleted = 100;
			break;
		case 2:
			linesDeleted = 300;
			break;
		case 3:
			linesDeleted = 500;
			break;
		default: // tetris
			linesDeleted = 800;
			break;
		}
		
	}

	void printParts(){
		setTextColor(WHITE);
		gotoxy(7, 0);
		for (int i = 0; i < 4; i++)
			cout << " ";
		gotoxy(7, 0);
		cout << partsNum++;

	}

	int getLinesDeleted() { return linesDeleted; }

	void setMax(int x, int max){ maxValueY[x - 1] = max; }
	int getMax(int x) { return maxValueY[x - 1]; }

	void setDistance(Shape& currentShape,int minYshape)
	{
		int currentMax=ROWS+Board_Gap, currentX;
		for (int i = 0; i < currentShape.SIZE; i++){
			currentX = currentShape.shape[i].getX();
			if (getMax(currentX)<currentMax)
				currentMax = getMax(currentX);
		}
		distance = currentMax - minYshape;
	}
	int getDistance(){ return distance; }

	void updateScoreValue(int addition) { scoreValue += addition; };
	int getScoreValue() { return scoreValue; }

	bool isLarger(int currentX, int maxY) // checks and updates the max array
	{
		if (maxY < maxValueY[currentX - 1]){
			setMax(currentX, maxY);
			return true;
		}
		return false;
	}

	void printScore(){
		setTextColor(LIGHTGREEN);
		gotoxy(35, 0);
		for (int i = 0; i < 10; i++)
			cout << " ";
		gotoxy(35, 0);
		cout << getScoreValue();
	}


	Score(){
		scoreValue = 0;
		linesDeleted = 0;
		partsNum = 1;
		speedCounter = 0;
		for (int i = 0; i < COLUMNS; i++)
			maxValueY[i] = ROWS + Board_Gap;
	}

	
};

#endif _Score_H