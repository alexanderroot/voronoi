#include <iostream>
#include <fstream>
#include "DataTypes.h"
#include "Utils.h"
#include <cmath>
#include <assert.h>

using namespace std;

bool equality(double x, double y, unsigned int accuracy) {
    return static_cast<int>(round(pow(10.0, accuracy) * x)) == static_cast<int>(round(pow(10.0, accuracy) * y));
}


void test_circle() 
{
    Point center;
    double radius = 0;
    Point a = Point(1.0, 0.0), b = Point(0.0, 0.0), c = Point(0.5, 0.5);
    assert(circle(a, b, c, &radius, &center));
    cout << center.xcor << " " << center.ycor << endl;
    cout << radius << endl;

    assert(equality(center.xcor, 0.50000, 4));
    assert(equality(center.ycor, 0.00000, 4));
    assert(equality(radius, 0.5000, 4));
    cout << "Test passed!" << endl;
}



int main(int argc, char* argv[])
{
    test_circle();
    return 0;
}