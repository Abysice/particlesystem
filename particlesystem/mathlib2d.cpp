/*
 * Adam Bysice 
 * File: mathlib2d.cpp
 * --------------------
 * This file contains the implementation
 * for the mathlib2D classes.
 */

const int MAX_SPD = 4;

#include <math.h>
#include <stdlib.h>
#include "Mathlib2D.h"

point2D::point2D (float x, float y) {
	this->x = x;
	this->y = y;
}

float point2D::getX() {
	return this->x;
}

float point2D::getY() {
	return this->y;
}

void point2D::setX(float x) {
	if (x > 500)
		this->x = int(x)%500;
	else if (x < 0)
		this->x = 500 - int(x)%500;
	else 
		this->x = x;
}

void point2D::setY(float y) {
	if (y > 500)
		this->y = int(y)%500;
	else if (y < 0)
		this->y = 500 - int(y)%500;
	else 
		this->y = y;
}

vec2D:: vec2D (float x, float y) {
	if (x < MAX_SPD && x > -MAX_SPD)
	{
		this->magnitude = sqrt((x*x) + (y*y));
		this->x = x;
	}
	else if (x >= MAX_SPD) {
		this->x = MAX_SPD;
	}
	else if (x <= -MAX_SPD) {
		this->x = -MAX_SPD;
	}
	//this->x = x;
	if (y < MAX_SPD && y > -MAX_SPD)
	{
		this->magnitude = sqrt((x*x) + (y*y));
		this->y = y;
	}
	else if (y <= -MAX_SPD) {
		this->y = -MAX_SPD;
	}
	else if (y >= MAX_SPD) {
		this->y = MAX_SPD;
	}
	//this->y = y;
	this->magnitude = sqrt((this->x*this->x) + (this->y*this->y));
}

float vec2D::length() {
	this->magnitude = sqrt((x*x) + (y*y));
	return magnitude;
}

void vec2D::setX(float x) {
	if (x < MAX_SPD && x > -MAX_SPD)
	{
		this->magnitude = sqrt((x*x) + (y*y));
		this->x = x;
	}
	else if (x >= MAX_SPD) {
		this->x = MAX_SPD;
	}
	else if (x <= -MAX_SPD) {
		this->x = -MAX_SPD;
	}
	
}

void vec2D::setY(float y) {
	if (y < MAX_SPD && y > -MAX_SPD)
	{
		this->magnitude = sqrt((x*x) + (y*y));
		this->y = y;
	}
	else if (y < -MAX_SPD) {
		this->y = -MAX_SPD;
	}
	else if (y > MAX_SPD) {
		this->y = MAX_SPD;
	}
	
}

float vec2D::getX() {
	return this->x;
}

float vec2D::getY() {
	return this->y;
}

colour2D::colour2D(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}

float gdistance(point2D p1, point2D p2) {
	float d = sqrt((p2.getX() - p1.getX())*(p2.getX() - p1.getX()) +(p2.getY() - p1.getY())*(p2.getY() - p1.getY()));
	return d;
}

//same as distance, except without the square root, usefull for relative distance
float fdistance(point2D p1, point2D p2) {
	float d = (p2.getX() - p1.getX())*(p2.getX() - p1.getX()) + (p2.getY() - p1.getY())*(p2.getY() - p1.getY());
	return d;
}

//takes a vector and returns its normalized unit length representation.
vec2D normalize(vec2D v1) {
	vec2D vec(v1.getX()/v1.magnitude, v1.getY()/v1.magnitude);
	return vec;
}

// Legacy function, reduces size of vectors 
vec2D vecGCD(vec2D v1) {
	if (abs(v1.getX()) >= abs(v1.getY())) {
		v1.setX(v1.getX()/10); //abs(v1.getX()));
		v1.setY(v1.getY()/10); //abs(v1.getX()));
	}
	else {
		v1.setY(v1.getY()/10);  //abs(v1.getY()));
		v1.setX(v1.getX()/10); //abs(v1.getY()));
	}

	return v1;
}

//multiplies a provided vector by a scalar
vec2D vecMult(vec2D v1, int s) {
	vec2D vec(v1.getX()*s, v1.getY()*s);
	return vec;
}

//takes two points, p1 = (x1, y1) and p2 = (x2, y2), and returns the vector
vec2D createVec(point2D p1, point2D p2) {
	vec2D someVec(p2.getX() - p1.getX(),  p2.getY() - p1.getY());
	return someVec;
}

//takes a point, p, and a vector, v, and returns the point (p + v)
point2D movePoint(point2D p1, vec2D v1) {
	point2D vecMov(p1.getX() + (v1.getX()*v1.magnitude), p1.getY() + (v1.getY()*v1.magnitude)); 
	return vecMov;
}

//calculates the collision of 2 vectors
void collision2D (point2D p1, point2D p2, vec2D *v1, vec2D *v2) {
	double  m21, dvx2, a, x21, y21, vx21, vy21, fy21, sign, vx_cm, vy_cm, R;
	//elasticity constant testing use only
	R = 1;

	m21=(1);
    x21=p2.getX()-p1.getY();
    y21=p2.getY()-p1.getY();
    vx21=v2->getX()-v1->getX();
    vy21=v2->getY()-v1->getY();
	vx_cm = (1*v1->getX()+1*v2->getX())/(1+1);
	vy_cm = (1*v1->getY()+1*v2->getY())/(1+1);   
	//return old velocities if not approaching
	if ( (vx21*x21 + vy21*y21) >= 0) return;
	//the following statements to avoid a zero divide; 
	fy21 = 1.0E-12*fabs(y21);
	if ( fabs(x21) < fy21 ) {  
		if (x21 < 0) { 
			sign = -1;
		}
		else {
			sign = 1;
			}
		x21 = fy21*sign; 
	} 
	//update velocities
	a = y21/x21;
	dvx2 = -2*(vx21 +a*vy21)/((1+a*a)*(1+m21)) ;
	v2->setX(v2->getX()+dvx2);
	v2->setY(v2->getY()+a*dvx2);
	v1->setX(v1->getX()-m21*dvx2);
	v1->setY(v1->getY()-a*m21*dvx2);	
	//velocity correction for inelastic collisions 
	//Still being tested
	v1->setX((v1->getX()-vx_cm)*R + vx_cm);
	v1->setY((v1->getY()-vy_cm)*R + vy_cm);
	v2->setX((v2->getX()-vx_cm)*R + vx_cm);
	v2->setY((v2->getY()-vy_cm)*R + vy_cm);
}

//returns greater psize
float greater (float s1, float s2) {
	if (s1 > s2)
		return s1;
	else
		return s2;
}