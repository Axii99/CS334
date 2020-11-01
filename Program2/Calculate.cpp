#include <iostream>
#include <math.h>
#include "Calculate.h"

using namespace std;

Point selector;
bool GridOn;
bool CurveOn;

float LineFormula(Line line, Point point) {
	float u = line.p1.y - line.p2.y;
	float v = line.p2.x - line.p1.x;
	float w = line.p1.x * line.p2.y - line.p2.x * line.p1.y;
	float h = sqrt(pow(u, 2) + pow(v, 2));
	float result = (u * point.x + v * point.y + w) / h;
	return result;
}

float getGroupProduct(int g, Point p) {
	if (selector.x < 0 || selector.y < 0) {
		cout << "selector undefined" << endl;
		return 0;
	}
	if (g == 1) {
		//Group A
		float product = 1.0;
		for (int i = 0; i < lineNum; i++) {
			if (allLines[i].group == 1) {
				product *= LineFormula(allLines[i], p);
			}
		}
		return product;
	}
	else if (g == 2) {
		//Group B
		float product = 1.0;
		for (int i = 0; i < lineNum; i++) {
			if (allLines[i].group == 2) {
				product *= LineFormula(allLines[i], p);
			}
		}
		return product;
	}
	else
	{
		cout << "Group Error" << endl;
		return 0;
	}
}

float getS() {
	if (lineNum == 0) {
		cout << "No Lines, s error" << endl;
		return 999;
	}
	float Pa = getGroupProduct(1, selector);
	float Pb = getGroupProduct(2, selector);
	cout << "Pa: " << Pa << "Pb" << Pb << endl;
	return Pa / (Pa - Pb);
}


void DrawGrid(int div) {
	if (selector.x < 0 || selector.y < 0) {
		cout << "selector undefined" << endl;
		return;
	}

	float interval = 800.0 / div;
	float s = getS();
	for (float i = 1.0; i < div; i++) {
		glPointSize(2.0);

		glBegin(GL_POINTS);
		for (float j = 1.0; j < div; j++) {
			float x = i * interval;
			float y = j * interval;
			Point temp = {x, y};
			float flag = (1 - s) * getGroupProduct(1, temp) + s * getGroupProduct(2, temp);
			if (flag < 0) {
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else if (flag > 0) {
				glColor3f(1.0f, 1.0f, 0.0f);
			}
			else {
				glColor3f(0.0f, 0.0f, 0.0f);
			}

			glVertex2f(temp.x, temp.y);
		}
		glEnd();
		glFlush();
	}
}

void DrawCurve(int div) {
	if (selector.x < 0 || selector.y < 0) {
		cout << "selector undefined" << endl;
		return;
	}

	float interval = 800.0 / div;
	float s = getS();
	glColor3f(1.0f, 1.0f, 1.0f);
	for (float i = 0.0; i < div; i++) {

		for (float j = 0.0; j < div; j++) {
			float x = i * interval;
			float y = j * interval;
			Point p0 = { x, y };
			Point p1 = { (i+1) * interval, y };
			Point p2 = { (i + 1) * interval, (j + 1) * interval };
			Point p3 = { x, (j + 1) * interval};
			Square current = { p0, p1, p2, p3 };
			float flags[4];
			flags[0] = (1 - s) * getGroupProduct(1, p0) + s * getGroupProduct(2, p0);
			flags[1] = (1 - s) * getGroupProduct(1, p1) + s * getGroupProduct(2, p1);
			flags[2] = (1 - s) * getGroupProduct(1, p2) + s * getGroupProduct(2, p2);
			flags[3] = (1 - s) * getGroupProduct(1, p3) + s * getGroupProduct(2, p3);
			if ((flags[0] < 0 && flags[1] > 0 && flags[2] > 0 && flags[3] > 0)
				|| (flags[0] > 0 && flags[1] < 0 && flags[2] < 0 && flags[3] < 0)) {
				glBegin(GL_LINES);
				glVertex2f((p0.x + p1.x)/2.0, p0.y);
				glVertex2f(p0.x, (p0.y + p3.y) / 2.0);
				glEnd();
			}
			else if ((flags[0] > 0 && flags[1] < 0 && flags[2] > 0 && flags[3] > 0)
					|| (flags[0] < 0 && flags[1] > 0 && flags[2] < 0 && flags[3] < 0)) {
				glBegin(GL_LINES);
				glVertex2f((p0.x + p1.x) / 2.0, p0.y);
				glVertex2f(p1.x, (p1.y + p2.y) / 2.0);
				glEnd();
			}
			else if ((flags[0] > 0 && flags[1] > 0 && flags[2] < 0 && flags[3] > 0)
				 || (flags[0] < 0 && flags[1] < 0 && flags[2] > 0 && flags[3] < 0)) {
				glBegin(GL_LINES);
				glVertex2f((p2.x + p3.x) / 2.0, p2.y);
				glVertex2f(p2.x, (p1.y + p2.y) / 2.0);
				glEnd();
			}
			else if ((flags[0] > 0 && flags[1] > 0 && flags[2] > 0 && flags[3] < 0)
				|| (flags[0] < 0 && flags[1] < 0 && flags[2] < 0 && flags[3] > 0)) {
				glBegin(GL_LINES);
				glVertex2f((p2.x + p3.x) / 2.0, p3.y);
				glVertex2f(p3.x, (p0.y + p3.y) / 2.0);
				glEnd();
			}
			else if ((flags[0] > 0 && flags[1] > 0 && flags[2] < 0 && flags[3] < 0)
				     || (flags[0] < 0 && flags[1] < 0 && flags[2] > 0 && flags[3] > 0)) {
				glBegin(GL_LINES);
				glVertex2f(p0.x, (p0.y + p3.y) / 2.0);
				glVertex2f(p1.x, (p1.y + p2.y) / 2.0);
				glEnd();
			}
			else if ((flags[0] > 0 && flags[1] < 0 && flags[2] < 0 && flags[3] > 0)
				|| (flags[0] < 0 && flags[1] > 0 && flags[2] > 0 && flags[3] < 0)) {
				glBegin(GL_LINES);
				glVertex2f((p0.x + p1.x) / 2.0, p0.y);
				glVertex2f((p2.x + p3.x) / 2.0, p3.y);
				glEnd();
			}
			else if (flags[0] > 0 && flags[1] < 0 && flags[2] > 0 && flags[3] < 0) {
				glBegin(GL_LINES);
				glVertex2f((p0.x + p1.x) / 2.0, p0.y);
				glVertex2f(p1.x, (p1.y + p2.y) / 2.0);
				glVertex2f((p2.x + p3.x) / 2.0, p3.y);
				glVertex2f(p3.x, (p0.y + p3.y) / 2.0);
				glEnd();
			}
			else if (flags[0] < 0 && flags[1] > 0 && flags[2] < 0 && flags[3] > 0) {
				glBegin(GL_LINES);
				glVertex2f((p0.x + p1.x) / 2.0, p0.y);
				glVertex2f(p3.x, (p0.y + p3.y) / 2.0);				
				glVertex2f((p2.x + p3.x) / 2.0, p3.y);
				glVertex2f(p1.x, (p1.y + p2.y) / 2.0);
				glEnd();
			}
		}

		glFlush();
	}
	cout << "Curve On" << endl;
}