#pragma once
#include <math.h>

const double PI = 3.141592653589793238463;

class Point
{
public:
    Point() {};
    Point(double a, double b, double c) :x(a), y(b), z(c) {};
public:
    double x;
    double y;
    double z;
};

Point getPoint(double u, double v)
{
    double r = 0.8;
    double x = r * sin(PI * v) * cos(PI*2 * u);
    double y = r * sin(PI * v) * sin(PI*2 * u);
    double z = r * cos(PI * v);
    return Point(x, y, z);
}

double getDist2D(float x1, float y1, float x2, float y2) {
    double result = sqrt((x1 - x2)* (x1 - x2) + (y1 - y2)* (y1 - y2));
    return result;
}

double dotProduct2D(float x1, float y1, float x2, float y2) {
    return x1*x2 + y1*y2;
}

double getDist3D(Point p1, Point p2) {
    double result = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
    return result;
}

double getLen3D(Point p) {
    double result = sqrt(p.x * p.x + p.y * p.y+ p.z * p.z);
    return result;
}

double dotProduct3D(Point p1, Point p2) {
    return p1.x * p2.x + p1.y * p2.y+ p1.z * p2.z;
}

double getAngle(Point p1, Point p2) {
    return acos(dotProduct3D(p1, p2) / (getLen3D(p1) * getLen3D(p2)));
}

Point convertPoint(double angle, double x, double y, double z, Point p, bool flag) {
    double s = sin(angle/ 180 * PI);
    double c = cos(angle / 180 * PI);
    x = x / sqrt(x * x + y * y + z * z);
    y = y / sqrt(x * x + y * y + z * z);
    z = z / sqrt(x * x + y * y + z * z);
    if (!flag) {
        double new_x = (x * x * (1 - c) + c) * p.x + (x * y * (1 - c) - z * s) * p.y + (x * z * (1 - c) + y * s) * p.z;
        double new_y = (y * x * (1 - c) + z * s) * p.x + (y * y * (1 - c) + c) * p.y + (y * z * (1 - c) - x * s) * p.z;
        double new_z = (x * z * (1 - c) - y * s) * p.x + (z * y * (1 - c) + x * s) * p.y + (z * z * (1 - c) + c) * p.z;
        return Point(new_x, new_y, new_z);
    }
    else {
        double new_x = (x * x * (1 - c) + c) * p.x + (y * x * (1 - c) + z * s) * p.y + (x * z * (1 - c) - y * s) * p.z;
        double new_y = (x * y * (1 - c) - z * s) * p.x + (y * y * (1 - c) +c) * p.y + (z * y * (1 - c) + x * s) * p.z;
        double new_z = (x * z * (1 - c) + y * s) * p.x + (y * z * (1 - c) - x * s) * p.y + (z * z * (1 - c) + c) * p.z;
        return Point(new_x, new_y, new_z);
    }

}