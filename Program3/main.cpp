#include <iostream>
#include "GL/freeglut.h"
#include "cube.h"

using namespace std;


Cube cubelist[5];
GLfloat startX;
GLfloat startY;

GLfloat rotateX = 0.0f;
GLfloat rotateY = 0.0f;
GLfloat transX = 0.0f;
GLfloat transY = 0.0f;
GLfloat scaleX =0.0f;
GLfloat scaleY = 0.0f;

bool horizon = false;
bool perspective = false;
void initCubes() {
    rotateX = 0.0f;
    rotateY = 0.0f;
    transX = 0.0f;
    transY = 0.0f;
    scaleX = 0.0f;
    scaleY = 0.0f;
    horizon = false;
    perspective = false;
    cubelist[0]= {
        Point{-0.5f, -1.0f, 1.0f},
        Point{0.5f, -1.0f, 1.0f},
        Point{0.5f, -1.0f, 2.0f},
        Point{-0.5f, -1.0f, 2.0f},
        Point{-0.5f, -2.0f, 1.0f},
        Point{0.5f, -2.0f, 1.0f},
        Point{0.5f, -2.0f, 2.0f},
        Point{-0.5f, -2.0f, 2.0f},
    };

    cubelist[1] = {
        Point{-2.5f, -1.0f, 1.0f},
        Point{-1.5f, -1.0f, 1.0f},
        Point{-1.5f, -1.0f, 2.0f},
        Point{-2.5f, -1.0f, 2.0f},
        Point{-2.5f, -2.0f, 1.0f},
        Point{-1.5f, -2.0f, 1.0f},
        Point{-1.5f, -2.0f, 2.0f},
        Point{-2.5f, -2.0f, 2.0f},
    };

    cubelist[2] = {
        Point{1.5f, -1.0f, 1.0f},
        Point{2.5f, -1.0f, 1.0f},
        Point{2.5f, -1.0f, 2.0f},
        Point{1.5f, -1.0f, 2.0f},
        Point{1.5f, -2.0f, 1.0f},
        Point{2.5f, -2.0f, 1.0f},
        Point{2.5f, -2.0f, 2.0f},
        Point{1.5f, -2.0f, 2.0f},
    };

    cubelist[3] = {
        Point{1.5f, -1.0f, -1.0f},
        Point{2.5f, -1.0f, -1.0f},
        Point{2.5f, -1.0f, 0.0f},
        Point{1.5f, -1.0f, 0.0f},
        Point{1.5f, -2.0f, -1.0f},
        Point{2.5f, -2.0f, -1.0f},
        Point{2.5f, -2.0f, 0.0f},
        Point{1.5f, -2.0f, 0.0f},
    };

    cubelist[4] = {
        Point{-2.5f, 3.0f, -2.0f},
        Point{-1.5f, 3.0f, -2.0f},
        Point{-1.5f, 3.0f, -1.0f},
        Point{-2.5f, 3.0f, -1.0f},
        Point{-2.5f, 2.0f, -2.0f},
        Point{-1.5f, 2.0f, -2.0f},
        Point{-1.5f, 2.0f, -1.0f},
        Point{-2.5f, 2.0f, -1.0f},
    };
}

void CubeLines(int i) {
    glBegin(GL_QUADS);
    glVertex3f(cubelist[i].vertexList[0].x, cubelist[i].vertexList[0].y, cubelist[i].vertexList[0].z);
    glVertex3f(cubelist[i].vertexList[1].x, cubelist[i].vertexList[1].y, cubelist[i].vertexList[1].z);
    glVertex3f(cubelist[i].vertexList[2].x, cubelist[i].vertexList[2].y, cubelist[i].vertexList[2].z);
    glVertex3f(cubelist[i].vertexList[3].x, cubelist[i].vertexList[3].y, cubelist[i].vertexList[3].z);
    glVertex3f(cubelist[i].vertexList[4].x, cubelist[i].vertexList[4].y, cubelist[i].vertexList[4].z);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(cubelist[i].vertexList[4].x, cubelist[i].vertexList[4].y, cubelist[i].vertexList[4].z);
    glVertex3f(cubelist[i].vertexList[5].x, cubelist[i].vertexList[5].y, cubelist[i].vertexList[5].z);
    glVertex3f(cubelist[i].vertexList[6].x, cubelist[i].vertexList[6].y, cubelist[i].vertexList[6].z);
    glVertex3f(cubelist[i].vertexList[7].x, cubelist[i].vertexList[7].y, cubelist[i].vertexList[7].z);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(cubelist[i].vertexList[2].x, cubelist[i].vertexList[2].y, cubelist[i].vertexList[2].z);
    glVertex3f(cubelist[i].vertexList[3].x, cubelist[i].vertexList[3].y, cubelist[i].vertexList[3].z);
    glVertex3f(cubelist[i].vertexList[7].x, cubelist[i].vertexList[7].y, cubelist[i].vertexList[7].z);
    glVertex3f(cubelist[i].vertexList[6].x, cubelist[i].vertexList[6].y, cubelist[i].vertexList[6].z);
    
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(cubelist[i].vertexList[0].x, cubelist[i].vertexList[0].y, cubelist[i].vertexList[0].z);
    glVertex3f(cubelist[i].vertexList[1].x, cubelist[i].vertexList[1].y, cubelist[i].vertexList[1].z);
    glVertex3f(cubelist[i].vertexList[5].x, cubelist[i].vertexList[5].y, cubelist[i].vertexList[5].z);
    glVertex3f(cubelist[i].vertexList[4].x, cubelist[i].vertexList[4].y, cubelist[i].vertexList[4].z);   
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(cubelist[i].vertexList[0].x, cubelist[i].vertexList[0].y, cubelist[i].vertexList[0].z);
    glVertex3f(cubelist[i].vertexList[3].x, cubelist[i].vertexList[3].y, cubelist[i].vertexList[3].z);
    glVertex3f(cubelist[i].vertexList[7].x, cubelist[i].vertexList[7].y, cubelist[i].vertexList[7].z);
    glVertex3f(cubelist[i].vertexList[4].x, cubelist[i].vertexList[4].y, cubelist[i].vertexList[4].z);
    glEnd();
    glBegin(GL_QUADS);
    glVertex3f(cubelist[i].vertexList[1].x, cubelist[i].vertexList[1].y, cubelist[i].vertexList[1].z);
    glVertex3f(cubelist[i].vertexList[2].x, cubelist[i].vertexList[2].y, cubelist[i].vertexList[2].z);
    glVertex3f(cubelist[i].vertexList[6].x, cubelist[i].vertexList[6].y, cubelist[i].vertexList[6].z);
    glVertex3f(cubelist[i].vertexList[5].x, cubelist[i].vertexList[5].y, cubelist[i].vertexList[5].z);
    glEnd();
}

void DrawCubes(void)
{
    glColor3f(0.5f, 0.5, 0.5);
    for (int i = 0; i < 5; ++i) {
        CubeLines(i);
    }

}

void DrawHorizon() {
    glColor3f(1.0f, 0, 0);
    glBegin(GL_LINES);
    glVertex3d(-1000, 0, 0);
    glVertex3d(1000, 0, 0);
    glEnd();
    glFlush();
}

void PerspectiveLines(int i) {
    glBegin(GL_LINES);
    glVertex3f(cubelist[i].vertexList[0].x, cubelist[i].vertexList[0].y, -500);
    glVertex3f(cubelist[i].vertexList[3].x, cubelist[i].vertexList[3].y, 500);
    glVertex3f(cubelist[i].vertexList[1].x, cubelist[i].vertexList[1].y, -500);
    glVertex3f(cubelist[i].vertexList[2].x, cubelist[i].vertexList[2].y, 500);
    glVertex3f(cubelist[i].vertexList[4].x, cubelist[i].vertexList[4].y, -500);
    glVertex3f(cubelist[i].vertexList[7].x, cubelist[i].vertexList[7].y, 500);
    glVertex3f(cubelist[i].vertexList[5].x, cubelist[i].vertexList[5].y, -500);
    glVertex3f(cubelist[i].vertexList[6].x, cubelist[i].vertexList[6].y, 500);
    glEnd();
}

void DrawPerspective() {
    glColor3f(1.0f, 0, 1.0f);
    for (int i = 0; i < 5; ++i) {
        PerspectiveLines(i);
    }
}

void mouseMotion(int x, int y) {
    if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
        GLfloat finalX = x;
        GLfloat finalY = glutGet(GLUT_SCREEN_HEIGHT) - y;
        scaleX = (GLfloat)(finalX - startX) / 200.0f;
        scaleY = (GLfloat)(finalY - startY) / 200.0f;
        glutPostRedisplay();
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        GLfloat finalX = x;
        GLfloat finalY = glutGet(GLUT_SCREEN_HEIGHT) - y;
        transX = (int)(finalX - startX);
        transY = (int)(finalY - startY);
        glutPostRedisplay();
    }
    else
    {
        GLfloat finalX = x;
        GLfloat finalY = glutGet(GLUT_SCREEN_HEIGHT) - y;
        rotateX = (int)(finalX - startX) % 360;
        rotateY = (int)(finalY - startY) % 360;
        glutPostRedisplay();
    }
}

void mouseDown(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        startX = x;
        startY = glutGet(GLUT_SCREEN_HEIGHT) - y;
        cout << x << ","<<y << endl;
        glutMotionFunc(mouseMotion);
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {

        glutMotionFunc(NULL);
    }
    
}





void menu(int num) {  // num: id of the added item
    switch (num) {
    case 0:
        glClearColor(0.0, 0.0, 0.0, 0.0);//设置清除颜色
        glClear(GL_COLOR_BUFFER_BIT);//把窗口清除为当前颜色
        glClearDepth(1.0);//指定深度缓冲区中每个像素需要的值
        glClear(GL_DEPTH_BUFFER_BIT);
        glutMouseFunc(NULL);
        
        initCubes();
        glutPostRedisplay();
        glutSwapBuffers();
        break;
    case 1:
        glutMouseFunc(mouseDown);
        break;
    case 2:
        glutMouseFunc(mouseDown);
        break;
    case 3:
        glutMouseFunc(mouseDown);
        break;
    case 4:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(30, 1.0, 1.0, 100.01);
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutPostRedisplay();
        break;
    case 5:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, 1.0, 1.0, 100.01);
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutPostRedisplay();
        break;
    case 6:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, 1.0, 1.0, 100.01);
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutPostRedisplay();
        break;
    case 7:
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(75, 1.0, 1.0, 100.01);
        gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glutPostRedisplay();
        break;
    case 8:
        horizon = !horizon;
        glutPostRedisplay();
        break;
    case 9:
        perspective = !perspective;
        glutPostRedisplay();
        break;
    default:
        break;
    }
}

void createMenu(void) {

    int angles = glutCreateMenu(menu);
    glutAddMenuEntry("30", 4);
    glutAddMenuEntry("45", 5);
    glutAddMenuEntry("60", 6);
    glutAddMenuEntry("75", 7);

    glutCreateMenu(menu);
    glutAddMenuEntry("Reset", 0);
    glutAddMenuEntry("Show Horizon", 8);
    glutAddMenuEntry("Show Perspective", 9);
    glutAddSubMenu("Set Aperture",angles);
    glutAttachMenu(GLUT_RIGHT_BUTTON);  // bind to the event: clicking the right button
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    //Rotate
    if (rotateX >= 0) {
       glRotatef(fabs(rotateX), 1, 0, 0);
    }
    else
    {
        glRotatef(fabs(rotateX), -1, 0, 0);
    }

    if (rotateY >= 0) {
        glRotatef(fabs(rotateY), 0, 1, 0);
    }
    else
    {
        glRotatef(fabs(rotateY), 0, -1, 0);
    }

    //Translate
    glTranslatef(transX/100, transY/100, 0);

    //Scale
    GLfloat xscale = 1.0f;
    GLfloat yscale = 1.0f;
    xscale += (scaleX);
    glScalef(xscale, xscale, xscale);
    DrawCubes();
    if (perspective) {
        DrawPerspective();
    }


    

    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    if (horizon) {
        DrawHorizon();
    }
    glFlush();


    glutSwapBuffers();
}

void changeSize(int x, int y) {

    int dis = x > y ? y : x;
    glViewport(0, 0, dis, dis);

}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    initCubes();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(600, 800);
    glutInitWindowPosition(200, 200);
    glViewport(0, 0, 600, 600);

    glutCreateWindow("Program3");

    createMenu();
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, 1.0, 500.01); // aperture, aspect, near, far
    gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutMouseFunc(mouseDown);

    glutMainLoop();

    return 0;

}