#include <iostream>
#include <math.h>
#include <list>
#include <vector>
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

double Dt = 0.5;
bool Ft = false;
bool arcDraw = false;
bool DeCadraw = false;

vector<Point> FtPoints;
vector<Point> ClipPoints;
vector<Point> HermitePoints;
vector<Point> HermiteBezier;
Point ps[30];
Point tangentPoints[30];
int pindex = 0;
int tpindex = 0;
bool setTP = false;
bool drawHermite = false;



double svalue = 1;
bool fromRight = true;


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
        Point b = getPoint(u + ustep, 0.5);
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
        Point b = getPoint(0, v + vstep);
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
    for (int i = 0; i < pindex - 1; i++) {
        Point p1 = ps[i];
        Point p2 = ps[i + 1];
        double omega = getAngle(p1, p2);
        glBegin(GL_LINE_STRIP);
        for (double t = 0.0; t <= 1; t = t + 0.02) {
            double sx = sin((1 - t) * omega) / sin(omega) * p1.x + sin(t * omega) / sin(omega) * p2.x;
            double sy = sin((1 - t) * omega) / sin(omega) * p1.y + sin(t * omega) / sin(omega) * p2.y;
            double sz = sin((1 - t) * omega) / sin(omega) * p1.z + sin(t * omega) / sin(omega) * p2.z;
            glVertex3f(sx, sy, sz);
        }
        glEnd();
    }
}


void drawFt(vector<Point> pv, double dt) {
    if (pv.size() == 1) {
        return;
    }

    glColor3f(1, 1, 0);
    vector<Point> nextlevel;
    for (int i = 0; i < pv.size() - 1; i++) {
        Point p1 = pv[i];
        Point p2 = pv[i + 1];
        double omega = getAngle(p1, p2);
        double sx = sin((1 - dt) * omega) / sin(omega) * p1.x + sin(dt * omega) / sin(omega) * p2.x;
        double sy = sin((1 - dt) * omega) / sin(omega) * p1.y + sin(dt * omega) / sin(omega) * p2.y;
        double sz = sin((1 - dt) * omega) / sin(omega) * p1.z + sin(dt * omega) / sin(omega) * p2.z;
        Point tp = Point(sx, sy, sz);
        nextlevel.push_back(tp);
    }
    if (nextlevel.size() == 1) {
        FtPoints.push_back(nextlevel[0]);
    }
    drawFt(nextlevel, dt);
}

void drawFtHermite(vector<Point> pv, double dt) {
    if (pv.size() == 1) {
        return;
    }

    vector<Point> nextlevel;
    for (int i = 0; i < pv.size() - 1; i++) {
        Point p1 = pv.at(i);
        Point p2 = pv.at(i+1);
        double omega = getAngle(p1, p2);
        double sx = sin((1 - dt) * omega) / sin(omega) * p1.x + sin(dt * omega) / sin(omega) * p2.x;
        double sy = sin((1 - dt) * omega) / sin(omega) * p1.y + sin(dt * omega) / sin(omega) * p2.y;
        double sz = sin((1 - dt) * omega) / sin(omega) * p1.z + sin(dt * omega) / sin(omega) * p2.z;
        Point tp = Point(sx, sy, sz);
        nextlevel.push_back(tp);
    }
    if (nextlevel.size() == 1) {
        HermiteBezier.push_back(nextlevel[0]);
    }
    drawFtHermite(nextlevel, dt);
}

void drawDeCasteljau(vector<Point> pv) {
    if (pv.size() == 1) {
        glColor3f(0, 0, 0.5);
        glPointSize(8);
        glBegin(GL_POINTS);
        glVertex3f(pv[0].x, pv[0].y, pv[0].z);
        glEnd();


        glColor3f(1, 1, 0);
        return;
    }

    glColor3f(1, 1, 0);
    vector<Point> nextlevel;
    for (int i = 0; i < pv.size() - 1; i++) {
        Point p1 = pv[i];
        Point p2 = pv[i + 1];
        double omega = getAngle(p1, p2);
        if (pv.size() == pindex) { glColor3f(0, 0, 1); }
        glBegin(GL_LINE_STRIP);
        for (double t = 0.0; t <= 1; t = t + 0.02) {
            //if (t >= svalue) { continue; }
            double sx = sin((1 - t) * omega) / sin(omega) * p1.x + sin(t * omega) / sin(omega) * p2.x;
            double sy = sin((1 - t) * omega) / sin(omega) * p1.y + sin(t * omega) / sin(omega) * p2.y;
            double sz = sin((1 - t) * omega) / sin(omega) * p1.z + sin(t * omega) / sin(omega) * p2.z;
            glVertex3f(sx, sy, sz);
        }
        glEnd();

        double sx = sin((1 - Dt) * omega) / sin(omega) * p1.x + sin(Dt * omega) / sin(omega) * p2.x;
        double sy = sin((1 - Dt) * omega) / sin(omega) * p1.y + sin(Dt * omega) / sin(omega) * p2.y;
        double sz = sin((1 - Dt) * omega) / sin(omega) * p1.z + sin(Dt * omega) / sin(omega) * p2.z;
        Point tp = Point(sx, sy, sz);
        glColor3f(0, 0, 0.5);
        glBegin(GL_POINTS);
        glVertex3f(sx, sy, sz);
        glEnd();
        glColor3f(1, 1, 0);
        nextlevel.push_back(tp);
    }
    drawDeCasteljau(nextlevel);
}

void drawClipDeCasteljau(vector<Point> pv) {
    if (pv.size() == 1) {
        glColor3f(0, 0, 0.5);
        glPointSize(8);
        glBegin(GL_POINTS);
        glVertex3f(pv[0].x, pv[0].y, pv[0].z);
        glEnd();


        glColor3f(1, 1, 0);
        return;
    }

    glColor3f(1, 1, 0);
    vector<Point> nextlevel;
    for (int i = 0; i < pv.size() - 1; i++) {
        Point p1 = pv[i];
        Point p2 = pv[i + 1];
        double omega = getAngle(p1, p2);
        if (pv.size() == pindex) { glColor3f(0, 0, 1); }
        glColor3f(1, 1, 0);
        glBegin(GL_LINE_STRIP);
        for (double t = 0.0; t <= 1; t = t + 0.02) {
            if ((i > 0 || t > svalue) && fromRight) { continue; }
            else if ((i > 0 || t < svalue) && !fromRight) { continue; }
            double sx = sin((1 - t) * omega) / sin(omega) * p1.x + sin(t * omega) / sin(omega) * p2.x;
            double sy = sin((1 - t) * omega) / sin(omega) * p1.y + sin(t * omega) / sin(omega) * p2.y;
            double sz = sin((1 - t) * omega) / sin(omega) * p1.z + sin(t * omega) / sin(omega) * p2.z;
            glVertex3f(sx, sy, sz);
        }
        glEnd();

        double sx = sin((1 - svalue) * omega) / sin(omega) * p1.x + sin(svalue * omega) / sin(omega) * p2.x;
        double sy = sin((1 - svalue) * omega) / sin(omega) * p1.y + sin(svalue * omega) / sin(omega) * p2.y;
        double sz = sin((1 - svalue) * omega) / sin(omega) * p1.z + sin(svalue * omega) / sin(omega) * p2.z;
        Point tp = Point(sx, sy, sz);
        glColor3f(0, 0, 0.5);
        if (i == 0) {
            glBegin(GL_POINTS);
            glVertex3f(sx, sy, sz);
            glEnd();
            glColor3f(1, 1, 0);
            ClipPoints.push_back(tp);
        }
        nextlevel.push_back(tp);
    }
    drawClipDeCasteljau(nextlevel);
}

void drawTangent() {

    glColor3f(0, 1, 1);
    for (int i = 0; i < pindex; i++) {
        Point p1 = ps[i];
        Point p2 = tangentPoints[i];
        double omega = getAngle(p1, p2);
        glBegin(GL_LINE_STRIP);
        for (double t = 0.0; t <= 1; t = t + 0.02) {
            double sx = sin((1 - t) * omega) / sin(omega) * p1.x + sin(t * omega) / sin(omega) * p2.x;
            double sy = sin((1 - t) * omega) / sin(omega) * p1.y + sin(t * omega) / sin(omega) * p2.y;
            double sz = sin((1 - t) * omega) / sin(omega) * p1.z + sin(t * omega) / sin(omega) * p2.z;
            glVertex3f(sx, sy, sz);
        }
        glEnd();
    }
}

void generateHermitePoints() {
    HermitePoints.clear();
    HermitePoints.push_back(ps[0]);
    HermitePoints.push_back(tangentPoints[0]);
    if (pindex != tpindex) {
        return;
    }
    for (int i = 1; i < pindex; i++) {
        Point p1 = ps[i];
        Point p2 = tangentPoints[i];
        double omega = getAngle(p1, p2);
        double temp = -1;
        double sx = sin((1 - temp) * omega) / sin(omega) * p1.x + sin(temp * omega) / sin(omega) * p2.x;
        double sy = sin((1 - temp) * omega) / sin(omega) * p1.y + sin(temp * omega) / sin(omega) * p2.y;
        double sz = sin((1 - temp) * omega) / sin(omega) * p1.z + sin(temp * omega) / sin(omega) * p2.z;
        glColor3f(1, 0, 0);
        glPointSize(5.0);
        glBegin(GL_POINTS);
        glVertex3f(sx, sy, sz);
        glEnd();
        Point tp = Point(sx, sy, sz);
        HermitePoints.push_back(tp);
        HermitePoints.push_back(ps[i]);
        HermitePoints.push_back(tangentPoints[i]);
    }
    glColor3f(1, 0, 0);
    glPointSize(5.0);

    glBegin(GL_POINTS);
    for (int i = 0; i < tpindex; i++) {
        glVertex3f(tangentPoints[i].x, tangentPoints[i].y, tangentPoints[i].z);
    }
    glEnd();
    
}

void drawHermiteLines() {
    if (pindex != tpindex) {
        return;
    }
    glColor3f(0, 1, 0);
    for (int i = 0; i < HermitePoints.size()-1; i++) {
        Point p1 = HermitePoints.at(i);
        Point p2 = HermitePoints.at(i+1);
        double omega = getAngle(p1, p2);
        glBegin(GL_LINE_STRIP);
        for (double t = 0.0; t <= 1; t = t + 0.02) {
            double sx = sin((1 - t) * omega) / sin(omega) * p1.x + sin(t * omega) / sin(omega) * p2.x;
            double sy = sin((1 - t) * omega) / sin(omega) * p1.y + sin(t * omega) / sin(omega) * p2.y;
            double sz = sin((1 - t) * omega) / sin(omega) * p1.z + sin(t * omega) / sin(omega) * p2.z;
            glVertex3f(sx, sy, sz);
        }
        glEnd();
    }
}

void drawHermiteBezier() {
    HermiteBezier.clear();
    if (HermitePoints.size() <= 2) {
        return;
    }
    for (int i = 0; i < HermitePoints.size()-3; i = i + 3) {
        
        for (double dt = 0; dt <= 1; dt = dt + 0.02) {
            vector<Point> pv;
            pv.push_back(HermitePoints.at(i));
            pv.push_back(HermitePoints.at(i+1));
            pv.push_back(HermitePoints.at(i+2));
            pv.push_back(HermitePoints.at(i+3));
            drawFtHermite(pv, dt);
            if (i >= HermitePoints.size()-2) { break; }
        }
    }

    glColor3f(2, 1, 1);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < HermiteBezier.size(); i++) {
        glVertex3f(HermiteBezier[i].x, HermiteBezier[i].y, HermiteBezier[i].z);
    }
    glEnd();

}

void init()
{
    glClearColor(0, 1, 1, 1);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 1, 1, 1);
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat light_position[] = { 1.0f, 1.0f, -2.0f, 0.0f };
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
    rotX = 1;
    rotY = 0;
    rotZ = 0;
    mouseX = 0;
    mouseY = 0;
    currentX = 0;
    currentY = 0;
    tempRotx = 0;
    tempRoty = 0;
    tempRotz = 0;
    rotAngle = 0; 
    tempRotAngle = 0;
    arcDraw = false;
    DeCadraw = false;
    Ft = false;
    pindex = 0;
    tpindex = 0;
    Dt = 0.5;
    svalue = 1;
    drawHermite = false;
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

    drawTangent();
    if (drawHermite) {
        
        generateHermitePoints();
        drawHermiteLines();
        drawHermiteBezier();
    }

    vector<Point> pv;
    for (int i = 0; i < pindex; i++) {
        pv.push_back(ps[i]);
    }
    if (arcDraw) {
        drawArcs();

    }
    if (pv.size() > 0 && DeCadraw) {
        drawClipDeCasteljau(pv);
    }

    FtPoints.clear();
    ClipPoints.clear();
    if (Ft && pv.size() > 0) {

        if (svalue > 1) {
            for (double dt = 0; dt <= svalue; dt = dt + 0.02) {
                //if (dt >= svalue) { continue; }
                drawFt(pv, dt);
            }
        }
        else if (svalue <= 0) {
            for (double dt = svalue; dt <= 1; dt = dt + 0.02) {
                //if (dt >= svalue) { continue; }
                drawFt(pv, dt);
            }
        }
        else {
            for (double dt = 0; dt <= 1; dt = dt + 0.02) {
            if (fromRight && dt >= svalue) { continue; }
            else if (!fromRight && dt <= svalue) { continue; }
                drawFt(pv, dt);
            }
        }
        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < FtPoints.size(); i++) {
            glVertex3f(FtPoints[i].x, FtPoints[i].y, FtPoints[i].z);
        }
        glEnd();
    }

    //cout << pv.size() << endl;

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
        if (!setTP) {
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
        else
        {
            for (int i = 0; i < pindex; i++) {
                double d = getDist2D(tangentPoints[i].x, tangentPoints[i].y, tp.x, tp.y);
                if (d < 0.05) {
                    if (target == -1) {
                        target = i;
                    }
                    else if (d < getDist2D(tangentPoints[target].x, tangentPoints[target].y, tp.x, tp.y)) {
                        target = i;
                    }
                }
            }
            if (target >= 0) {
                cout << "Dragging" << endl;
                tangentPoints[target] = tp;
            }
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

        tempRotAngle = delta / 100 + rotAngle;
        tempRotx = rotX - mouseY;
        tempRoty = rotY + mouseX;
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
    for (int i = index; i < pindex - 1; i++) {
        ps[i] = ps[i + 1];
    }
    pindex--;
}

void mouseDown(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseX = x - 400;
        mouseY = 400 - y;
        cout << mouseX << "," << mouseY << endl;
        if (glutGetModifiers() == (GLUT_ACTIVE_SHIFT | GLUT_ACTIVE_CTRL)) {
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
        else if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
            float z = pow(0.8, 2) - pow(mouseX / 400.0, 2) - pow(mouseY / 400.0, 2);
            if (z < 0) { return; }
            if (pindex >= 29) {
                return;
            }

            z = sqrt(abs(z));

            Point tp = Point(mouseX / 400.0, mouseY / 400.0, -z);
            if (setTP) {
                if (tpindex >= pindex) { return; }
                tangentPoints[tpindex] = convertPoint(-rotAngle, rotX, rotY, rotZ, tp, FALSE);
                tpindex++;
            }
            else
            {
                ps[pindex] = convertPoint(-rotAngle, rotX, rotY, rotZ, tp, FALSE);
                pindex++;
            }
            
            cout << "Defining:" << mouseX / 400.0 << ", " << mouseY / 400.0 << ", " << z << endl;
            cout << "A:" << rotAngle << ", " << rotX << ", " << rotY << endl;
            
        }
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (tempRotx > 0 || tempRotAngle > 0) {
            rotX = tempRotx / sqrt(tempRotx * tempRotx + tempRoty * tempRoty + tempRotz * tempRotz);
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


void setTvalue() {
    double temp = -1;
    while (temp < 0 || temp > 1) {
        cout << "Please enter t value (0-1):" << endl;
        cin >> temp;
    }
    Dt = temp;
    cout << "New t value: " << Dt << endl;
}

void setSvalue() {
    double temp = -1;
    cout << "Please enter s value :" << endl;
    cin >> temp;
    svalue = temp;
    cout << "New s value: " << svalue << endl;
}

void menu(int num) {  // num: id of the added item
    switch (num) {
    case 0:
        Reset();
        //glutMouseFunc(NULL);
        //glutMotionFunc(NULL);
        glutPostRedisplay();
        break;
    case 1:
        glutMouseFunc(mouseDown);
        glutMotionFunc(mouseMotion);
        glutPostRedisplay();
        glutSwapBuffers();
        break;
    case 2:
        if (arcDraw) {
            arcDraw = false;
        }
        else { arcDraw = true; }
        glutPostRedisplay();
        break;
    case 3:
        if (DeCadraw) {
            DeCadraw = false;
        }
        else { DeCadraw = true; }
        glutPostRedisplay();
        break;
    case 4:
        if (Ft) {
            Ft = false;
        }
        else { Ft = true; }
        glutPostRedisplay();
        break;
    case 5:
        setTvalue();
        glutPostRedisplay();
        break;
    case 6:
        setSvalue();
        glutPostRedisplay();
        break;
    case 7:
        if (setTP) {
            cout << "Control points" << endl;
            setTP = false;
        }
        else {
            cout << "Tangent points" << endl;
            setTP = true;
        }
        glutPostRedisplay();
        break;
    case 8:
        if (drawHermite) {
            drawHermite = false;
        }
        else {
            cout << "Draw Hermite" << endl;
            drawHermite = true;
        }
        glutPostRedisplay();
        break;
    case 9:
        if (fromRight) {
            fromRight = false;
        }
        else {
            cout << "Clip from beginning" << endl;
            fromRight = true;
        }
        glutPostRedisplay();
    default:
        return;
    }
}

void createMenu(void) {


    glutCreateMenu(menu);
    glutAddMenuEntry("Clear", 0);
    glutAddMenuEntry("Adjust", 1);
    glutAddMenuEntry("Polygon", 2);
    glutAddMenuEntry("Decasteljau", 3);
    glutAddMenuEntry("Clip from Beginning/Right", 9);
    glutAddMenuEntry("F(t)", 4);
    glutAddMenuEntry("Set s value", 6);
    glutAddMenuEntry("Set&Move Control/Tangent points", 7);
    glutAddMenuEntry("Draw Hermite", 8);
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