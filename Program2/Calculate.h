#pragma once
#include "Point.h"
#include "GL/freeglut.h"
#include <stdio.h>

using namespace std;

extern Point selector;
extern bool GridOn;
extern bool CurveOn;


float LineFormula(Line, Point);
float getS();
float getGroupProduct(int, Point);
void DrawGrid(int div);
void DrawCurve(int div);