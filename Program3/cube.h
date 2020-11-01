#pragma once
#include "GL/freeglut.h"
struct  Point
{
	GLfloat x;
	GLfloat y;
	GLfloat z;

};

struct Cube {
	Point vertexList[8];
	/*
	Point v1;
	Point v2;
	Point v3;
	Point v4;
	Point v5;
	Point v6;
	Point v7;
	Point v8;
	*/
};