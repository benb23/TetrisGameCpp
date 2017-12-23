#ifndef _ScoreBar_H
#define _ScoreBar_H
#include "TetrisBoard.h"

class Score{
	int scoreValue, distance, linesDeleted, partsNum, speedCounter;
	int maxValueY[COLUMNS];
public:
	enum { VERY_SLOW = -2, SLOW, NORMAL, HIGH, VERY_HIGH };		// Speed const

	Score();

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

	void printSpeed();

	void setLinesDeleted(int howMany, const Shape& currentShape);

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

	void setDistance(Shape& currentShape, int minYshape);

	int getDistance(){ return distance; }

	void updateScoreValue(int addition) { scoreValue += addition; };
	
	int getScoreValue() { return scoreValue; }

	// The function checks and updates the max array
	bool isLarger(int currentX, int maxY) 
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

	
};

#endif _Score_H