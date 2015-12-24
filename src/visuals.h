#ifndef __VISUALS__
#define __VISUALS__ 

#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4

class Point {
	float x; 
	float y; 
	float z;
public:
	Point(float, float, float);
	~Point();
};

class Faces {
	Point *p1;
	Point *p2;
	Point *p3;
public:
	Faces(float, float, float, float, float, float, float, float, float);
	~Faces();
};

class myModel {
	vector <Point> v;
	vector <Point> vn;
	vector <Faces> f;
public:
	myModel();
	~myModel();

};

typedef struct face {
	int vtx[3];
} face;

typedef struct model {
	// Point obj_points[10];
	face obj_faces[4];
	int vertices;
	int faces;
} model;


//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int w, int h);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup();
// Set up the OpenGL state machine and create a light source

void Idle();

void ReadFile(model *, char *);
//Function for reading a model file

void DisplayModel(model);
// Function for displaying a model

void Keyboard(unsigned char key,int x,int y);
// Function for handling keyboard events.

void Mouse(int button,int state,int x,int y); 
// Function for handling mouse events

void MenuSelect(int choice);

#endif