// CMath.c
// By Kyle O'Rourke
// 1/17/2018

#include "CMath.h"

float myabs(float absIN); // ABS Function prototype.

float arctangent2(float y, float x)
{
    float ny = myabs(y);
    float pi = 3.1415926;
    float theta;
    if (x > 0) {
        float r = (x - ny)/(x + ny);
        theta = (0.1963*(r*r*r))-(0.9817*r)+(pi/4);
    }
    else{
        float r = (x + ny)/(x - ny);
        theta = (0.1963*(r*r*r))-(0.9817*r)+((3*pi)/4);
    }
    if (y < 0){
        theta = -1*theta;
    }
    return theta;
}

float enorm(float px, float py, float qx, float qy)
{
    float dX = myabs(qx - px);
    float dY = myabs(qy - py);

    float g, e;
    if (dX > dY) {
        g = dX;
        e = dY;
    } else {
        g = dY;
        e = dX;
    }
    float t, r, s;
    int i;
    for (i = 0; i < 1; i++) {
        t = e / g;
        r = t*t;
        s = r / (4.0 + r);
        g = g + (2 * s * g);
        e = e*s;
    }
    return g;
}



// Convert given number to its ABS value.
float myabs(float absIN)
{
    if (absIN == 0) {
        return 0;
    }
    if (absIN < 0){
        absIN = -1*absIN;
    }
    return (absIN);
}
