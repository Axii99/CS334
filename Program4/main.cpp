#include <iostream>
#include <math.h>
#include "GL/freeglut.h"
#include "Sphere.h"

using namespace std;


float rotX = 0;
float rotY = 0;
float rotZ = 0;
float rotAngle = 0;
int mouseX = 0;
int mouseY = 0;
int currentX = 0;
int currentY = 0;
double tempRotx = 0;
double tempRoty = 0;
double tempRotz = 0;
double tempRotAngle = 0;


bool arcDraw = false;

Point ps[30];
int pindex = 0;

void drawWire()
{
    int uStepsNum = 12;
    int vStepNum = 12;
    double ustep = 1 / (double)uStepsNum, vstep = 1 / (double)vStepNum;
    double u = 0, v = 0;
    //top
    
    for (int i = 0; i < uStepsNum; i++)
    {
        glBegin(GL_LINE_LOOP);
        Point a = getPoint(0, 0);
        glVertex3f(a.x, a.y, a.z);
        Point b = getPoint(u, vstep);
        glVertex3f(b.x, b.y, b.z);
        Point c = getPoint(u + ustep, vstep);
        glVertex3f(c.x, c.y, c.z);
        glEnd();
        u += ustep;
        
        
    }
    
    //mid
    u = 0, v = vstep;
    for (int i = 0; i < vStepNum - 1; i++)
    {
        for (int j = 0; j < uStepsNum; j++)
        {
            glBegin(GL_LINE_LOOP);
            Point a = getPoint(u, v);
            Point b = getPoint(u + ustep, v);
            Point c = getPoint(u + ustep, v + vstep);
            Point d = getPoint(u, v + vstep);
            glVertex3f(a.x, a.y, a.z);
            glVertex3f(b.x, b.y, b.z);
            glVertex3f(c.x, c.y, c.z);
            glVertex3f(d.x, d.y, d.z);
            glEnd();
            u += ustep;


        }
        v += vstep;
    }
    //bottom
    
    u = 0;
    for (int i = 0; i < uStepsNum; i++)
    {
        
        glBegin(GL_LINE_LOOP);
        Point a = getPoint(0, 1);
        Point b = getPoint(u, 1 - vstep);
        Point c = getPoint(u + ustep, 1 - vstep);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        glVertex3f(c.x, c.y, c.z);
        glEnd();

    }

    glColor3f(1.0, 1.0, 0.0);
    glPointSize(5);
    glBegin(GL_POINTS);
    Point p1 = getPoint(0, 0);
    Point p2 = getPoint(0, 1);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glEnd();

    u = 0;
    glColor3f(0.0, 2.0, 0.0);
    glLineWidth(5);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < uStepsNum; i++)
    {
        Point a = getPoint(u, 0.5);
        Point b = getPoint(u+ustep, 0.5);       
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        u += ustep;
    }
    glEnd();

    v = 0;
    glBegin(GL_LINES);
    glColor3f(0.0, 2.0, 1.0);
    for (int i = 0; i < vStepNum; i++)
    {
        Point a = getPoint(0, v);
        Point b = getPoint(0, v+vstep);
        glVertex3f(a.x, a.y, a.z);
        glVertex3f(b.x, b.y, b.z);
        v += vstep;
    }
    glEnd();

    glLineWidth(1);
}


void drawPoints() {
    glColor3f(1, 1, 1);
    glPointSize(5.0);
 
    glBegin(GL_POINTS);
    for (int i = 0; i < pindex; i++) {
        glVertex3f(ps[i].x, ps[i].y, ps[i].z);
        //cout << ps[i].x << "," << ps[i].y << ", " << ps[i].z << endl;
    }
    glEnd();
}


void drawArcs() {
    glColor3f(0, 0, 1);
    for (int i = 0; i < pindex-1; i++) {
        Point p1 = ps[i];
        Point p2 = ps[i + 1];
        double omega = getAngle(p1, p2);
        glBegin(GL_LINES);
        for (double t = 0.0; t <= 1; t = t + 0.05) {
            double sx = sin((1 - t) * omega) / sin(omega) * p1.x + sin(t * omega) / sin(omega) * p2.x;
            double sy = sin((1 - t) * omega) / sin(omega) * p1.y + sin(t * omega) / sin(omega) * p2.y;
            double sz = sin((1 - t) * omega) / sin(omega) * p1.z + sin(t * omega) / sin(omega) * p2.z;
            glVertex3f(sx, sy, sz);
        }
        glEnd();
    }
}

void init()
{
    glClearColor(0, 1, 1, 1);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 1, 1, 1);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat light_position[] = {1.0f, 1.0f, -2.0f, 0.0f };
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 0.2f };
    GLfloat light_diffuse[] = { 0.2f, 0.2f, 0.2f, 0.2f };
    GLfloat light_specular[] = { 0.2f, 0.2f, 0.2f, 0.2f };

    GLfloat mat_ambient[] = { 0.0f, 0.0f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    rotX = 1;
    rotY = 0;
    rotZ = 0;
    mouseX = 0;
    mouseY = 0;
    tempRotx = 0;
    tempRoty = 0;
    //glDepthFunc(GL_LESS);
}

void Reset() {
    rotX = 0;
    rotY = 0;
    rotZ = 0;
    mouseX = 0;
    mouseY = 0;
    currentX = 0;
    currentY = 0;
    tempRotx = 0;
    tempRoty = 0;
    rotAngle = 0;
    arcDraw = false;
    pindex = 0;
    glutPostRedisplay();
}

void display() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    
    glLoadIdentity();
    
    


    glRotated(rotAngle, rotX, rotY, rotZ);

    glPointSize(10.0);
    glColor3f(0.0, 1.0, 1.5);
    glutSolidSphere(0.77, 40.0, 40.0);
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glColor3f(1.0, 0.0, 0.0);
    drawWire();
    drawPoints();
    if (arcDraw) {
        drawArcs();
    }





    glColor3f(1.0, 0.0, 0.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    //glLoadIdentity();
    glFlush();
    glutSwapBuffers();
}

void mouseMotion(int x, int y) {
    currentX = x - 400;
    currentY = 400 - y;
    if (glutGetModifiers() == GLUT_ACTIVE_CTRL) {
        float z = pow(0.82, 2) - pow(currentX / 400.0, 2) - pow(currentY / 400.0, 2);
        if (z < 0) { return; }
        z = sqrt(z);
        Point tp = Point(currentX / 400.0, currentY / 400.0, -z);
        tp = convertPoint(-rotAngle, rotX, rotY, rotZ, tp, FALSE);
        int target = -1;
        for (int i = 0; i < pindex; i++) {
            double d = getDist2D(ps[i].x, ps[i].y, tp.x, tp.y);
            if (d < 0.05) {
                if (target == -1) {
                    target = i;
                }
                else if (d < getDist2D(ps[target].x, ps[target].y, tp.x, tp.y)) {
                    target = i;
                }
            }
        }
        if (target >= 0) {
            cout << "Dragging" << endl;
            ps[target] = tp;
        }
    }
    else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
        
    }
    else
    {
        currentX = x - 400;
        currentY = 400 - y;
        
        double initDist = getDist2D(0, 0, mouseX, mouseY);
        double currentDist = dotProduct2D(mouseX, mouseY, currentX, currentY) / initDist;
        double delta = currentDist - initDist;
        if (abs(delta) < 1) {
            glutPostRedisplay();;
            return;
        }

        tempRotAngle = delta / 100+rotAngle;
        tempRotx = rotX - mouseY;
        tempRoty = rotY  + mouseX;
        //cout << "Moving: " << currentX << "," << currentY <<"Delta: "<< tempRotAngle <<"| RotX = "<< tempRotx << " | RotY =" << tempRoty<< endl;
        rotX = tempRotx / sqrt(tempRotx * tempRotx + tempRoty * tempRoty + tempRotz * tempRotz);
        rotY = tempRoty / sqrt(tempRotx * tempRotx + tempRoty * tempRoty + tempRotz * tempRotz);
        rotZ = tempRotz / sqrt(tempRotx * tempRotx + tempRoty * tempRoty + tempRotz * tempRotz);
        tempRotx = 0;
        tempRoty = 0;
        tempRotz = 0;
        rotAngle = tempRotAngle;
        tempRotAngle = 0;
    }
    glutPostRedisplay();
}

void removePoint(int index) {
    for (int i = index; i < pindex-1; i++) {
        ps[i] = ps[i + 1];
    }
    pindex--;
}

void mouseDown(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseX = x-400;
        mouseY = 400 - y;
        cout << mouseX << "," << mouseY << endl;
        if (glutGetModifiers() == (GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL)){
            float z = pow(0.8, 2) - pow(mouseX / 400.0, 2) - pow(mouseY / 400.0, 2);
            if (z < 0) { return; }
            z = sqrt(z);
            Point tp = Point(mouseX / 400.0, mouseY / 400.0, -z);
            tp = convertPoint(-rotAngle, rotX, rotY, rotZ, tp, FALSE);
            int target = -1;
            for (int i = 0; i < pindex; i++) {
                double d = getDist2D(ps[i].x, ps[i].y, tp.x, tp.y);
                if (d < 0.05) {
                    if (target == -1) {
                        target = i;
                    }
                    else if (d < getDist2D(ps[target].x, ps[target].y, tp.x, tp.y)) {
                        target = i;
                    }
                }
            }
            if (target >= 0) {
                cout << "Removing" << endl;
                removePoint(target);
            }
        }
        else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
            float z = pow(0.8, 2) - pow(mouseX / 400.0, 2) - pow(mouseY / 400.0, 2);
            if (z < 0 || pindex >= 29) {
                return;
            }
            else {
                z = sqrt(z);
            }
            Point tp = Point(mouseX / 400.0, mouseY / 400.0, -z);
            ps[pindex] = convertPoint(-rotAngle, rotX, rotY, rotZ, tp,FALSE);
            //ps[pindex] = tp;
            cout << "Defining:" << mouseX / 400.0 << ", " << mouseY / 400.0 << ", " << z << endl;
            cout << "A:" << rotAngle << ", " << rotX << ", " << rotY<< endl;
            pindex++;
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        if (tempRotx > 0|| tempRotAngle > 0) {
            rotX = tempRotx / sqrt(tempRotx*tempRotx+tempRoty*tempRoty+tempRotz*tempRotz);
            rotY = tempRoty / sqrt(tempRotx * tempRotx + tempRoty * tempRoty + tempRotz * tempRotz);
            rotZ = tempRotz / sqrt(tempRotx * tempRotx + tempRoty * tempRoty + tempRotz * tempRotz);
            tempRotx = 0;
            tempRoty = 0;
            tempRotz = 0;
            rotAngle = tempRotAngle;
            tempRotAngle = 0;
        }
    }
    glutPostRedisplay();
}

void changeSize(int x, int y) {

    int dis = x > y ? y : x;
    glViewport(0, 0, 800, 800);
}

void menu(int num) {  // num: id of the added item
    switch (num) {
    case 0:
        Reset();
        glutMouseFunc(NULL);
        glutMotionFunc(NULL);
        break;
    case 1:
        glutMouseFunc(mouseDown);
        glutMotionFunc(mouseMotion);
        break;
    case 2:
        arcDraw = !arcDraw;
        glutPostRedisplay();
        break;
    default:
        return;
    }
}

void createMenu(void) {
    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddMenuEntry("Adjust", 1);
    glutAddMenuEntry("Polygon", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(200, 200);
    glViewport(0, 0, 800, 800);
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glutCreateWindow("Program4");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutMouseFunc(mouseDown);
    glutMotionFunc(mouseMotion);

    createMenu();
    glutMainLoop();

    return 0;

}