#pragma once
#include "Point.h"
#include <iostream>
#include <malloc.h>
#include <math.h>

#define MAXNUM 32;

Line* allLines;
int lineNum;

void AddLine(Point start, Point end) {
	cout << "Add Line" << endl;
	allLines[lineNum].p1 = start;
	allLines[lineNum].p2 = end;
	allLines[lineNum].group = 0;
	lineNum++;
}

void initLines() {
	allLines = (Line*)malloc(32 * sizeof(Line));
	lineNum = 0;
}

void printLines() {
	if (lineNum == 0) {
		cout << "Nothing\n" << endl;
	}
	for (int i = 0; i < lineNum; i++) {
		Point start = allLines[i].p1;
		Point end = allLines[i].p2;
		cout << "(" << start.x << "," << start.y << ") -----" << " (" << end.x << "," << end.y << ") in Group: "<<allLines[i].group<<endl;
	}
	cout << endl;
}

void freeLines(){
	free(allLines);
}

void ClearLines() {
	lineNum = 0;
}

float dist(Point a, Point b) {
	float result = sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	return result;
}

int findClosestPoint(Point p) {
	float minDist = -1;
	int index = 0;
	
	for (int i = 0; i < lineNum; i++) {
		float temp1 = dist(p, allLines[i].p1);
		float temp2 = dist(p, allLines[i].p2);
		if (minDist < 0 || temp1 < minDist) {
			minDist = temp1;
			index = i;
		}
		if (minDist < 0 || temp2 < minDist) {
			minDist = temp2;
			index = i;
		}
	}
	
	return index;
}


float distLine(Line l , Point p) {
	float x1 = l.p1.x;
	float y1 = l.p1.y;
	float x2 = l.p2.x;
	float y2 = l.p2.y;
	float x0 = p.x;
	float y0 = p.y;
	float d = (fabs((y2 - y1) * x0 + (x1 - x2) * y0 + ((x2 * y1) - (x1 * y2)))) / (sqrt(pow(y2 - y1, 2) + pow(x1 - x2, 2)));
	return d;
}

int findClosestLine(Point p) {
	float minDist = -1;
	int index = 0;
	for (int i = 0; i < lineNum; i++) {
		float temp = distLine(allLines[i], p);
		if (minDist < 0 || temp < minDist) {
			minDist = temp;
			index = i;
		}
	}

	return index;
}

int findMin3(float a , float b , float c) {
	if (a > 5 && b > 5 && c > 5) {
		return -1;
	}
	cout << c << endl;
	if (a <= b) {
		if (a - 2< c) {
			return 0;
		}
		else {
			return 2;
		}
	}
	else {
		if (b - 2< c) {
			return 1;
		}
		else {
			return 2;
		}
	}
}