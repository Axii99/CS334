#pragma once

using namespace std;



struct Point {
	float x;
	float y;
};


struct Line {
	Point p1;
	Point p2;
	int group = 0; //0 undefined 1-group1, 2-group2
};

struct Square {
	/* p0-------p1
	   |        |
	   |        |
	   p3-------p2
	*/
	Point p0;
	Point p1;
	Point p2;
	Point p3;
};


extern Line* allLines;
extern int lineNum;

void initLines();
void AddLine(Point, Point);
void printLines();
void freeLines();
void ClearLines();
float dist(Point, Point);
int findClosestPoint(Point);
float distLine(Line, Point);
int findClosestLine(Point);
int findMin3(float, float, float);
