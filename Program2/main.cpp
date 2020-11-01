#pragma once
#include <iostream>
#include <thread>
#include "GL/freeglut.h"
#include "Point.h"
#include "main.h"
#include "Calculate.h"


#define BUFFSIZE 1024




using namespace std;


static int submenu;
GLuint selectBuf[BUFFSIZE];

Point startPoint = { 0, 0 };
Point mouseStart = { 0, 0 };
Point mouseCurrent = { 0, 0 };
bool mouseMode = 0;

//define line
bool isFirst = true;
int setGroup = 0;
//move line
int targetLine;
int inMove;
int moveFlag;

//draw
int division = 100;


static int nameIndex = 5;




void createMenu(void) {

	int selects = glutCreateMenu(menu);
	glutAddMenuEntry("Select A", 6);
	glutAddMenuEntry("Select B", 7);
	glutAddMenuEntry("Unselect", 8);
	glutAddMenuEntry("Define Selector", 9);
	glutAddMenuEntry("Move Selector", 10);


	int defines = glutCreateMenu(menu);
	glutAddMenuEntry("Define Line", 0);
	glutAddMenuEntry("Move Line", 1);
	glutAddMenuEntry("Delete Line", 2);
	glutAddMenuEntry("None", 3);
	glutAddMenuEntry("Reset", 4);
	glutAddMenuEntry("Exit", 5);

	int draw = glutCreateMenu(menu);
	glutAddMenuEntry("Draw Grid", 11);
	glutAddMenuEntry("Draw Curve", 13);

	glutCreateMenu(menu);
	glutAddSubMenu("Defines", defines);
	glutAddSubMenu("Selects", selects);
	glutAddSubMenu("Draw", draw);
	glutAddMenuEntry("Density", 12);
	glutAttachMenu(GLUT_RIGHT_BUTTON);  // bind to the event: clicking the right button
}

void display(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//cout << "display" <<endl;
	DrawAll(GL_RENDER);
	glutSwapBuffers();

}

void drawPoint(int x, int y) {
	glPointSize(5.0);
	glColor3f(1.0f, 0.5f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
	//cout << "drawPoint" << endl;
}

void drawLine(Line l) {
	float x1 = l.p1.x;
	float x2 = l.p2.x;
	float y1 = l.p1.y;
	float y2 = l.p2.y;
	float slope = (y2 - y1) * (1.0) / (x2 - x1);
	float edgey1 = y1 + (800 - x1) * slope;
	float edgey2 = y1 + (0 - x1) * slope;

	glColor3f(0.0f, 1.0f, 1.0f);
	if (l.group == 1) {
		glColor3f(1.0f, 0.0f, 1.0f);
	}
	else if (l.group == 2) {
		glColor3f(1.0f, 1.0f, 0.0f);
	}
	glBegin(GL_LINES);
	glVertex2d(800.0, edgey1);
	glVertex2d(0.0, edgey2);
	glEnd();

	glPointSize(5.0);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2d(x1, y1);
	glVertex2d(x2, y2);
	glEnd();

	glFlush();
}


void mouseDefine(int button, int state, int x, int y) {
	int height = GLUT_SCREEN_HEIGHT;
	if (state == GLUT_UP)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			// y的转换
			y = 800 - y;
			// 如果是第一次，不画线，并赋值false关闭
			if (isFirst)
			{
				isFirst = false;

			}
			else
			{

				AddLine(startPoint, Point{ (float)x, (float)y });
				DrawAll(GL_RENDER);
				printLines();
				isFirst = true;
			}
			// 存储该点
			startPoint.x = x;
			startPoint.y = y;
		}
	}
}


void DrawAll(GLenum mode)
{
	int index = 1;
	for (int i = 0; i < lineNum; i++) {

		if (mode == GL_SELECT) glPushName(index++);
		drawLine(allLines[i]);

	}
	//draw Selector
	if (selector.x >= 0) {
		drawPoint(selector.x, selector.y);
	}

	if (GridOn) {
		DrawGrid(division);
	}
	if (CurveOn) {
		DrawCurve(division);
	}
}


void startPicking(GLint xMouse, GLint yMouse) {

	GLint vp[4];
	glSelectBuffer(BUFFSIZE, selectBuf); //设置选择缓冲区
	glRenderMode(GL_SELECT); //激活选择模式
	glInitNames();   //初始化名字堆栈
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glGetIntegerv(GL_VIEWPORT, vp);
	//定义一个10×10的选择区域
	gluPickMatrix(GLdouble(xMouse), GLdouble((GLdouble)vp[3] - yMouse), 10.0, 10.0, vp);
	gluOrtho2D(0.0, 800, 0.0, 800);

	DrawAll(GL_SELECT);

	//恢复投影变换
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	//获得选择集并输出
	GLint nPicks = glRenderMode(GL_RENDER);
	cout << "Selecting: " << nPicks << endl;
}


void mouseMove(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouseStart.x = x;
		mouseStart.y = 800 - y;
		mouseCurrent.x = x;
		mouseCurrent.y = 800 - y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		inMove = 0;
	}
}

void mouseDelete(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (lineNum == 0) { return; }
		Point current = { (float)x, float(800 - y) };
		int index = findClosestLine(current);
		if (distLine(allLines[index], current) > 10) {
			printLines();
			cout << "too far" << distLine(allLines[index], current)<< endl;
			return;
		}
		for (int i = index; i < lineNum - 1; i++) {
			allLines[i] = allLines[i + 1];
		}
		lineNum--;
		printLines();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawAll(GL_RENDER);
		glutSwapBuffers();
	}
}

void mouseMotion(int x, int y) {
	if (mouseMode == 1) {
		if (x > 800 || x < 0 || y > 800 || y < 0) {
			return;
		}
		mouseCurrent.x = x;
		mouseCurrent.y = 800 - y;
		int distance = dist(mouseStart, mouseCurrent);
		//cout << x << "," << y << " dist: " << distance << endl;

		if (lineNum == 0) {
			return;
		}



		//Set move type
		//check vertex
		if (inMove == 0) {
			int closestpoint = findClosestPoint(mouseStart);
			int closestLine = findClosestLine(mouseStart);

			int flag = findMin3(dist(mouseStart, allLines[closestpoint].p1), dist(mouseStart, allLines[closestpoint].p2), distLine(allLines[closestLine], mouseStart));
			if (flag == 2) {
				moveFlag = 2;
				targetLine = closestLine;
			}
			else if (flag == 1) {
				moveFlag = 1;
				targetLine = closestpoint;
			}
			else if (flag == 0)
			{
				moveFlag = 0;
				targetLine = closestpoint;
			}
			else
			{
				return;
			}
			inMove = 1;
		}

		float dx = mouseCurrent.x - mouseStart.x;
		float dy = mouseCurrent.y - mouseStart.y;
		if (moveFlag == 0) {			
			allLines[targetLine].p1.x = mouseCurrent.x;
			allLines[targetLine].p1.y = mouseCurrent.y;
		}
		else if (moveFlag  == 1)
		{
			allLines[targetLine].p2.x = mouseCurrent.x;
			allLines[targetLine].p2.y = mouseCurrent.y;
		}
		else if (moveFlag == 2) {
			allLines[targetLine].p1.x += dx;
			allLines[targetLine].p1.y += dy;
			allLines[targetLine].p2.x += dx;
			allLines[targetLine].p2.y += dy;
			mouseStart.x = mouseCurrent.x;
			mouseStart.y = mouseCurrent.y;
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		printLines();
		DrawAll(GL_RENDER);
		glutSwapBuffers();
	}
}

void mouseGroup(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		if (lineNum == 0) { return; }
		Point current = { (float)x, float(800 - y) };
		int index = findClosestLine(current);
		if (distLine(allLines[index], current) > 10) {
			printLines();
			cout << "too far" << distLine(allLines[index], current) << endl;
			return;
		}
		allLines[index].group = setGroup;
		printLines();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawAll(GL_RENDER);
		glutSwapBuffers();
	}
	cout << "Current S: "<< getS() << endl;
}

void selectorDef(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		selector.x = x;
		selector.y = 800 - y;
		cout << "Current S: " << getS() << endl;
	}
}

void selectorMove(int x, int y) {
	Point current = { x, 800-y };
	if (dist(current, selector) < 5) {
		selector.x = current.x;
		selector.y = current.y;
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		DrawAll(GL_RENDER);
		glutSwapBuffers();
		cout << "Selector: (" << selector.x <<", "<<selector.y<<")"<< endl;
		cout << "Current S: " << getS() << endl;
	}

}


void menu(int num) {  // num: id of the added item
	switch (num) {
	case 0:
		cout << "Define" << endl;
		glutMouseFunc(mouseDefine);           // 鼠标回调
		break;
	case 1:
		cout << "Move" << endl;
		glutMouseFunc(mouseMove);
		glutMotionFunc(mouseMotion);
		break;
	case 2:
		cout << "Delete" << endl;
		glutMouseFunc(mouseDelete);
		break;
	case 3:
		cout << "None" << endl;
		glutMouseFunc(NULL);
		break;
	case 4:
		cout << "Reset" << endl;
		selector.x = -1;
		selector.y = -1;
		glClearColor(0.0, 0.0, 0.0, 0.0);//设置清除颜色
		glClear(GL_COLOR_BUFFER_BIT);//把窗口清除为当前颜色
		glClearDepth(1.0);//指定深度缓冲区中每个像素需要的值
		glClear(GL_DEPTH_BUFFER_BIT);
		glutSwapBuffers();
		ClearLines();
		break;
	case 5:
		cout << "Exit" << endl;
		exit(0);
		break;
	case 6:
		setGroup = 1;
		glutMouseFunc(mouseGroup);
		break;
	case 7:
		setGroup = 2;
		glutMouseFunc(mouseGroup);
		break;
	case 8:
		setGroup = 0;
		glutMouseFunc(mouseGroup);
		break;
	case 9:
		glutMouseFunc(selectorDef);
		break;
	case 10:
		glutMouseFunc(NULL);
		glutMotionFunc(selectorMove);
		break;
	case 11:
		if (GridOn) {
			GridOn = false;
		}
		else
		{
			GridOn = true;
		}
		glutPostRedisplay();
		break;
	case 12:
		changeDiv();
		break;
	case 13:
		if (CurveOn) {
			CurveOn = false;
		}
		else
		{
			CurveOn = true;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}

	mouseMode = num;
}

void resize(int width, int height) {
	glutReshapeWindow(800, 800);
}

void changeDiv() {
		cout << "Current Div: " << division << endl;
		cin >> division;
		cout<<"New Div:" << division << endl;
		glutPostRedisplay();
}

void initAll() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);      // 设置显示模式
	glutInitWindowSize(800, 800);   // 设置窗口大小
	glutInitWindowPosition(0, 0);   // 设置窗口位于屏幕位置

	

	initLines();
	printLines();
	GridOn = false;
	CurveOn = false;
	selector = { -1, -1 };
	DrawAll(GL_RENDER);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);              // 初始化
	
	initAll();
	glutCreateWindow("Program2");         // 创建窗口


	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // 设置渲染状态
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, 800, 0, 800);            // 矩阵变换

	//Menu Setting
	createMenu();

	glutReshapeFunc(resize);
	glutMotionFunc(mouseMotion);
	glutDisplayFunc(display);           // 显示回调



	glutMainLoop();



	return 0;
}