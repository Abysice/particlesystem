/* 
 * Adam Bysice 
 * File: main.cpp
 * ---------------------------
 * main program file, contains
 * most of the implementation
 */
#include <stdlib.h>
#include <iostream>
#include <gl/freeglut.h>
#include <time.h>
#include <random>
#include <gl/gl.h>
#include "Mathlib2D.h"
using namespace std;

// Constants
const int MAX_PARTICLES = 3000;

//global variables
bool pause = false;
bool collision = false;
unsigned int range = 50;
int currentSize = 100;
int attraction = 0;
bool dispMode = false;
point2D* boxList[MAX_PARTICLES];
vec2D* vecList[MAX_PARTICLES];
colour2D* colourList[MAX_PARTICLES];
point2D mousePoint(-200,-200);

//display function
void display(void)
{
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluOrtho2D(0,500,500,0);
	glPointSize(2);

	for(int i = 0; i <= currentSize; i++) {
		glPointSize(boxList[i]->psize);
		glColor3d(colourList[i]->r, colourList[i]->g, colourList[i]->b);

		glBegin(GL_POINTS);
			glVertex2f(boxList[i]->getX(), boxList[i]->getY());
		glEnd();
	}
	glutSwapBuffers();
	glutPostRedisplay();
}

//readjusts coordinate systems to match the "World"
void reshape(int x, int y) {
	gluOrtho2D(0,y,x,0);
}

// pmovement function.
void pMove1() {

	// attracts/repels each particle from the mouse position
	for(int i = 0; i <= currentSize; i++) {
		float dis = gdistance(mousePoint, *boxList[i]);
		if (dis <= range){
			vec2D disp = createVec(mousePoint, *boxList[i]);
			//vec2D disp2 = vecGCD(disp);
			vec2D disp2 = normalize(disp);
			vecList[i]->setX(vecList[i]->getX() - attraction*disp2.getX());
			vecList[i]->setY(vecList[i]->getY() - attraction*disp2.getY());
		}
		// if collision is on and the particle is moving
		if (collision && (vecList[i]->magnitude > 0)){
			point2D temp = movePoint(*boxList[i], *vecList[i]);
			for (int j = 0; j <= currentSize; j++) {
				// check if temp is already occupied by another box
				/*if (((abs(boxList[j]->getX() - temp.getX()) <= boxList[i]->psize) || 
					(abs(boxList[j]->getX() - temp.getX()) <= boxList[j]->psize)) && 
					((abs(boxList[j]->getY() - temp.getY()) <= boxList[i]->psize) || 
					(abs(boxList[j]->getY() - temp.getY()) <= boxList[j]->psize)))*/
				if ( gdistance(*boxList[j], temp) <= greater(boxList[i]->psize, boxList[j]->psize) ) {
					//preform a collision calculation with their two vectors
					collision2D(temp, *boxList[j], vecList[i], vecList[j]);
				}
			}
		}
	}

	// updates all particle positions
	for(int i = 0; i <= currentSize; i++) {
		// update the box positions with their new coordinates
		point2D temp = movePoint(*boxList[i], *vecList[i]);
		boxList[i]->setX(temp.getX());
		boxList[i]->setY(temp.getY());
		if (dispMode) {
			colourList[i]->r=vecList[i]->magnitude/5.66;
			colourList[i]->g=vecList[i]->magnitude/5.66;
			colourList[i]->b=vecList[i]->magnitude/5.66;
		}
	}
Sleep(50);
}

// runs when nothing else is happening
void idle() {
	if (!pause) {
		pMove1();
	}
}

//mouse button function
void mousebut(int btn, int state, int x, int y) {
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		attraction = 1;
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		attraction = -1;
	}
	else{
		attraction = 0;
	}
}

//tracks movement of the mouse
void motion(int x, int y) {
	mousePoint.setX(x);
	mousePoint.setY(y);
}

//keeps track of passive mouse positon
void passive(int x, int y) {
	mousePoint.setX(x);
	mousePoint.setY(y);
}

// initializes the particles randomly
void particleInit() {
	srand(time(NULL)); // Seed the time
	// initialize the points, vectors, and colours
	for(int i = 0; i <= currentSize; i++) {
		boxList[i] = new point2D(rand()%(500)+1, rand()%(500)+1);
		boxList[i]->psize = rand()%(3)+2;
		colourList[i] = new colour2D(0+(1-0)*rand()/((float)RAND_MAX),0+(1-0)*rand()/((float)RAND_MAX), 0+(1-0)*rand()/((float)RAND_MAX));
		vecList[i] = new vec2D(0, 0);
	}
}

// keyboard input function
void keyboard(unsigned char key, int x, int y) {
	if (key == 'p' || key == 'P'){
		// pause program
		pause = !pause;
	}
	else if (key == 'q' || key == 'Q') {
		//exit program
		exit(0);
	}
	else if (key == 'r' || key == 'R') {
		//redraw particles
		particleInit();
		if (collision){
			for(int i = 0; i <= currentSize; i++) {
				boxList[i]->psize = rand()%(5)+5;
			}
		}
	}
	else if (key == '+') {
		// increase range 
		range += 5;
	}
	else if (key == 'c' || key == 'C') {
		// enables collision
		if (!collision){
			for(int i = 0; i <= currentSize; i++) {
				boxList[i]->psize = rand()%(5)+5;
			}
		}
		else {
			for(int i = 0; i <= currentSize; i++) {
				boxList[currentSize]->psize = rand()%(3)+2;
			}
		}
		collision = !collision;
	}
	else if (key == '-' && range > 0) {
		//decrease range
		range -= 5;
	}
	else if(key == 'd' || key == 'D') {
		// changes display mode
		if (dispMode)
			particleInit();
		if (collision){
			for(int i = 0; i <= currentSize; i++) {
				boxList[i]->psize = rand()%(5)+5;
			}
		}
		dispMode = !dispMode;
	}
	else if((key == 's' || key == 'S') && (currentSize > 0)) {
		// removes point closes to the mouse
		int closest = 0;
		// find the point closest to the mouse
		for(int i = 0; i <= currentSize; i++) {
			if (fdistance(mousePoint, *boxList[i]) < fdistance(mousePoint, *boxList[closest])) {
				closest = i;
			}
		}
		// remove the point and shift everything closer
		for(int i = closest; i < currentSize; i++) {
			boxList[i] = boxList[i+1];
			vecList[i] = vecList[i+1];
			colourList[i] = colourList[i+1];
		}
		currentSize -= 1;
	}
	else if((key == 'w' || key == 'W') && (currentSize < MAX_PARTICLES)) {
		//randomly adds a pixel
		currentSize += 1;
		boxList[currentSize] = new point2D(rand()%(500)+1, rand()%(500)+1);
		boxList[currentSize]->psize = rand()%(3)+2;
		colourList[currentSize] = new colour2D(0.1+(1-0.1)*rand()/((float)RAND_MAX),0.1+(1-0.1)*rand()/((float)RAND_MAX), 0.1+(1-0.1)*rand()/((float)RAND_MAX));
		vecList[currentSize] = new vec2D(0, 0);
	}
	else if((key == 'a' || key == 'A') && (currentSize < MAX_PARTICLES)) {
		// add new dot at mouse pos
		currentSize += 1;
		boxList[currentSize] = new point2D(mousePoint.getX(), mousePoint.getY());
		boxList[currentSize]->psize = rand()%(3)+2;
		colourList[currentSize] = new colour2D(0.1+(1-0.1)*rand()/((float)RAND_MAX),0.1+(1-0.1)*rand()/((float)RAND_MAX), 0.1+(1-0.1)*rand()/((float)RAND_MAX));
		vecList[currentSize] = new vec2D(0, 0);
	}
}

/* main function - program entry point */
int main(int argc, char** argv)
{
	particleInit();

	printf("Left Mouse Button:  attract particles \n");
	printf("Right Mouse Button: repel particles \n");
	printf("Key 'p':            pause program \n");
	printf("Key 'r':            reset program \n");
	printf("Key 'q':            quits program \n");
	printf("Key '+':            increases range \n");
	printf("Key '-':            decreases range \n");
	printf("Key 'a':            adds particle \n");
	printf("Key 's':            removes particle \n");
	printf("Key 'd':            changes display mode \n");
	printf("Key 'w':            adds a particle randomly \n");
	printf("Key 'c':            toggle elastic collision \n");
	printf("WARNING: enabling collision slows down around 200 particles \n");

	glutInit(&argc, argv);		//starts up GLUT
	glutInitWindowSize(500,500);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Assignment 1");	//creates the window
	glutReshapeFunc(reshape);
	glutPassiveMotionFunc(passive);
	glutMouseFunc(mousebut);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutIdleFunc(idle);

	glutMainLoop();				//starts the event loop

	return(0);					//return may not be necessary on all compilers
}