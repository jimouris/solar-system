#ifndef __VISUALS__
#define __VISUALS__ 

#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#define POINTS 9122
#define NORMALS 9124
#define FACES 18240
#define CAM_FAR 10000
#define STARS 13

enum Light_t {SUN, STAR};

class Point {
public:
	float x; 
	float y; 
	float z;
};

typedef struct Face{
	int vtx[3];
} Face;

class Stars {
public:
	float starsShineSize[STARS];
	Point starsPosition[STARS];
	bool starsgrow[STARS];
	Point colour;
};

class myModel {
public:
	Point obj_points[POINTS]; /* all v */
	Point obj_normals[FACES]; /* all f */
  	Face obj_faces[NORMALS]; // array of all f
  	Face obj_norm[FACES];
	int vertices; // #v
	int faces; // #f

	Point position;
};


//-------- Functions --------------------------------

void Render();
// The function responsible for drawing everything in the 
// OpenGL context associated to a window. 

void Resize(int, int);
// Handle the window size changes and define the world coordinate 
// system and projection type

void Setup(char *);
// Set up the OpenGL state machine and create a light source

void Idle();

void ReadFile(char *);
//Function for reading a model file

void DisplayModel(myModel);
// Function for displaying a model

void Keyboard(unsigned char, int, int);
// Function for handling keyboard events.

void Mouse(int, int, int, int); 
// Function for handling mouse events
void MenuSelect(int);

void createSun(void);

#endif