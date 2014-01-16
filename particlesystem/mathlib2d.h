/* 
 * Adam Bysice 
 * File: Mathlib2D.h
 * ----------------------
 * Interface for 2D math library
 */

#ifndef _Mathlib2D_h
#define _Mathlib2D_h

#include <math.h>
#include <stdlib.h>

//represents a 2D point with floats/floats for x and y components
class point2D {
public:
	point2D(float x, float y);
	void setX(float x);
	void setY(float y);
	float getX();
	float getY();
	float psize;
private:
	float x;
	float y;

};

//represents a 2D vector with floats/floats for x, y and magnitude (i.e.,the vector’s length)
class vec2D {
public:
	vec2D(float x, float y);
	void setX(float x);
	void setY(float y);
	float getX();
	float getY();
	float length();
	float magnitude;
private:
	float x;
	float y;
	
};

//represents a colour using floats/floats for r, g, b components

class colour2D{
public:
	colour2D(float r, float g, float b);
	float r;
	float g;
	float b;
};

// returns the distance between two points

float gdistance(point2D p1, point2D p2);

//as distance, except without the square root, usefull for relative distance

float fdistance(point2D p1, point2D p2);

//takes a vector and returns its normalized unit length representation.

vec2D normalize(vec2D v1);

//multiplies a provided vector by a scalar

vec2D vecMult(vec2D v1, int s);

// uses GC to make 1 largest thingy

vec2D vecGCD(vec2D v1);

//takes two points, p1 = (x1, y1) and p2 = (x2, y2), and returns the vector

vec2D createVec(point2D p1, point2D p2);

//takes a point, p, and a vector, v, and returns the point (p + v)

point2D movePoint(point2D p1, vec2D v1);

//colision fuction, takes 2 points and 2 vectors and calculates their elastic collisions
//does not detect collision, only calculates IF they collided

void collision2D(point2D p1, point2D p2, vec2D *v1, vec2D *v2);

float greater(float s1, float s2);

#endif