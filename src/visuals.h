#ifndef __VISUALS__
#define __VISUALS__ 

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std;

#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#define POINTS 9122
#define NORMALS 9124
#define FACES 18240
#define CAM_FAR 10000
#define STARS 80
#define NORMAL_PLANETS 0

enum Light_t {SUN, STAR};

typedef struct Point {
	float x; 
	float y; 
	float z;
} Point;

typedef struct Face{
	int vtx[3];
} Face;

class Stars {
public:
	float starsSize[STARS];
	float starsShineSize[STARS];
	Point starsPosition[STARS];
	bool starsgrow[STARS];
	Point colour;
};

class Planets {
public:
	Point planetPos[4];
	Point colour[4];
};

class Model {
public:
	Point obj_points[POINTS]; /* all v */
	Point obj_normals[FACES]; /* all f */
  	Face obj_faces[NORMALS]; // array of all f
  	Face obj_norm[FACES];
	int vertices; // #v
	int faces; // #f

	Point position;
};

void Render(void);
void Resize(int, int);
void Setup(char *);
void Idle(void);
void ReadFile(char *);
void DisplayModel(Model);
void Keyboard(unsigned char, int, int);
void Mouse(int, int, int, int); 
void MenuSelect(int);
void createSun(void);
void createLightSource(float, float, Point, Point, Light_t);
void createStars(void);
void drawStars(void);
void shine(float &, bool &, float, float, float);
void planetMovement(void);

#endif