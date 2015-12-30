#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>

#include "GL/glut.h"   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions

myModel md;
static float tx = 0.0;
static float rotx = 0.0;
static bool animate = false;
static float red = 1.0;
static float green = 0.0;
static float blue = 0.0;
static bool grow = true;

static float shineSize = 400.0;

Stars starSystem;
enum Light_t {SUN, STAR};

using namespace std;

void createLightSource(float planetSize, float shineInitSize, Point position, Point colour, Light_t l) {
	glPushMatrix();
		glTranslatef(position.x, position.y, position.z);
		glColor3f(colour.x, colour.y, colour.z);
		glutSolidSphere(planetSize, 40, 40);
		if (l == SUN) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(colour.x, colour.y+0.2, colour.z, 0.1);
	  		glutWireSphere(shineInitSize, 40, 40);
			glDisable(GL_BLEND);
		} else if (l == STAR) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(colour.x, colour.y+0.2, colour.z, 0.1);
	  		glutSolidSphere(shineInitSize, 40, 40);
			glDisable(GL_BLEND);
		}
	glPopMatrix();
}

void createStars() {
	srand((unsigned) time(NULL));
	starSystem.colour.x = 1; starSystem.colour.y = 1; starSystem.colour.z = 1;
	for (int i = 0 ; i < STARS ; i++) {
		starSystem.starsgrow[i] = true;
		starSystem.starsShineSize[i] = rand()%4+ 2;
		starSystem.starsPosition[i].x = rand()%400-200; starSystem.starsPosition[i].y = rand()%400-200; starSystem.starsPosition[i].z = /*rand()%100*/ - 515;
	}
}

void drawStars() {
	for (int i = 0 ; i < STARS ; i++) {
		createLightSource(1, starSystem.starsShineSize[i], starSystem.starsPosition[i], starSystem.colour, STAR);	
	}
}

void Render() {    
	//CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window and the depth buffer
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	Point pos, col;
	pos.x = 0.0; pos.y = 0.0; pos.z = -515;
	col.x = 1.0; col.y = 0.8; col.z = 0.0;
	createLightSource(50, shineSize, pos, col, SUN);

	drawStars();
	// glPushMatrix();
	// 	glTranslatef(0, 0, -300);
	// 	glTranslatef(tx, 0.0, 0.0);
	// 	glRotatef(rotx, 1, 0, 0);

	// 	glColor3f(0.75, 0.35, 0.05);
	// 	DisplayModel(md);
	// glPopMatrix();



	glutSwapBuffers();	// All drawing commands applied to the hidden buffer, so now, bring forward the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Resize(int w, int h) { 
	// define the visible area of the window ( in pixels )
	if (h == 0) {
		h = 1;
	}
	// Setup viewing volume
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	glViewport( 0, 0, w, h);
	gluPerspective(45, (float)w/(float)h, 1, CAM_FAR);

    glMatrixMode(GL_MODELVIEW);
}

void shine(float &shine, bool &g, float upbound, float lowbound) {
	if (g) {
		shine += 0.1;
	} else {
		shine -= 0.1;
	}
	if (shine > upbound){
		g = false;
	} else if (shine < lowbound){
		g = true;
	}
}

void Idle() {
	if (animate) {
		// rotx += 0.4;

		shine(shineSize, grow, 420, 400);
		for (int i = 0 ; i < STARS ; i++) {
			shine(starSystem.starsShineSize[i], starSystem.starsgrow[i], 8, 4);
		}
	}

	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'q':
		case 'Q':
			exit(0);
			break;
		case 'a':
			tx -= 0.5f;
			break;
		case 'd':
			tx += 0.5f;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		animate = !animate;
		glutPostRedisplay();
	}
}

void Setup(char *path) { 
	ReadFile(path);
	createStars();

	//Parameter handling
	glShadeModel(GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  //renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);

	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { 0.0, 30.0, 50.0, 0.0 };
	  // GLfloat light_position[] = { 1000,1000,-3000, 10.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	   
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight );
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// glEnable(GL_COLOR_MATERIAL);
	// glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE );

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MenuSelect(int choice) {
	switch (choice) {
		case RED: 
			red = 1.0; 
			green = 0.0; 
			blue = 0.0;
			break;
		case GREEN: 
			red = 0.0; 
			green = 1.0; 
			blue = 0.0;
			break;
		case BLUE: 
			red = 0.0; 
			green = 0.0; 
			blue = 1.0;
			break;
		case WHITE: 
			red = 1.0; 
			green = 1.0; 
			blue = 1.0;
			break;
		default:
			break;
	}
}

void ReadFile(char *path) {
	md.vertices = POINTS; 
	md.faces = FACES;    
	char first[80];
	FILE *file = fopen(path, "r");
	if (file == NULL) {
		fprintf(stderr, "Error opening file\n");
		exit(EXIT_FAILURE);
	}
	int i = 0, j = 0, m = 0;
	while (!feof(file)) {
		if (fscanf(file, "%s  ", first) != 1) {
			fprintf(stderr, "error reading line header\n");
			exit(EXIT_FAILURE);
		}
		if ( !strcmp(first, "v") ){
			if (fscanf(file,"%f %f %f ", &md.obj_points[i].x, &md.obj_points[i].y, &md.obj_points[i].z) != 3) {
				fprintf(stderr, "error reading objpoints\n");
				exit(EXIT_FAILURE);
			}
			i++;
		} else if ( !strcmp(first, "vn") ){
			if (fscanf(file,"%f %f %f ", &md.obj_normals[m].x, &md.obj_normals[m].y, &md.obj_normals[m].z) != 3) {
				fprintf(stderr, "error reading objnormals\n");
				exit(EXIT_FAILURE);
			}
			m++;
		} else if ( !strcmp( first, "f")){
			if (fscanf(file,"%d//%d %d//%d %d//%d ", &md.obj_faces[j].vtx[0], &md.obj_norm[j].vtx[0], &md.obj_faces[j].vtx[1], &md.obj_norm[j].vtx[1], &md.obj_faces[j].vtx[2], &md.obj_norm[j].vtx[2]) != 6) {
				fprintf(stderr, "error reading faces\n");
				exit(EXIT_FAILURE);
			}
			j++;
		}
	}

	fclose(file);
}

void DisplayModel(myModel md) {
	glPushMatrix();

	glBegin(GL_TRIANGLES);
		for (int i = 0; i < md.faces; i++) {
			glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x, md.obj_points[md.obj_faces[i].vtx[0]-1].y, md.obj_points[md.obj_faces[i].vtx[0]-1].z);
			glNormal3f(md.obj_normals[md.obj_norm[i].vtx[0]-1].x, md.obj_normals[md.obj_norm[i].vtx[0]-1].y, md.obj_normals[md.obj_norm[i].vtx[0]-1].z);
			glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x, md.obj_points[md.obj_faces[i].vtx[1]-1].y, md.obj_points[md.obj_faces[i].vtx[1]-1].z);
			glNormal3f(md.obj_normals[md.obj_norm[i].vtx[1]-1].x, md.obj_normals[md.obj_norm[i].vtx[1]-1].y, md.obj_normals[md.obj_norm[i].vtx[1]-1].z);
			glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x, md.obj_points[md.obj_faces[i].vtx[2]-1].y, md.obj_points[md.obj_faces[i].vtx[2]-1].z);
			glNormal3f(md.obj_normals[md.obj_norm[i].vtx[2]-1].x, md.obj_normals[md.obj_norm[i].vtx[2]-1].y, md.obj_normals[md.obj_norm[i].vtx[2]-1].z);
		}
	glEnd();

	glPopMatrix();
}
