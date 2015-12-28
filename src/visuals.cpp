#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "GL/glut.h"   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions

myModel md;
static float tx = 0.0;
static float rotx = 0.0;
static bool animate = false;
static float red = 1.0;
static float green = 0.0;
static float blue = 0.0;

using namespace std;

Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}
float Point::getX() {
	return this->x;
}

float Point::getY() {
	return this->y;
}

float Point::getZ() {
	return this->z;
}

void Point::print(void) {
	cout << this->x << " " << this->y << " " << this->z << endl;
}

Faces::Faces(float v1, float vn1, float v2, float vn2, float v3, float vn3) {
	this->v = new Point(v1, v2, v3);
	this->vn = new Point(vn1, vn2, vn3);
}

Point* Faces::getV() {
	return this->v;
}

Point* Faces::getVn() {
	return this->vn;
}

void Faces::print(void) {
	this->v->print();
	this->vn->print();
}

void keimeno(const char *str, float size) {
	glPushMatrix();
	glScalef(size,size,size);

	for (size_t i=0;i<strlen(str);i++) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	}
	glPopMatrix();
}

void Render() {    
	//CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window and the depth buffer
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();

	glTranslatef(0, 0, -100);
	glTranslatef(tx, 0.0, 0.0);
	glRotatef(rotx, 0, 1, 0);

	//(01)             
	glColor3f(0.75, 0.35, 0.05);
	DisplayModel(md);

	// glColor3f(0.8, 0.4, 0.1);
	// glutSolidSphere(0.999, 30, 24);


	glutSwapBuffers();	// All drawing commands applied to the hidden buffer, so now, bring forward the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Resize(int w, int h) { 
	// define the visible area of the window ( in pixels )
	if (h == 0) {
		h = 1;
	}
	glViewport(0, 0, w, h); 

	// Setup viewing volume
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	gluPerspective(2.0, (float)w/(float)h, 1.0, 500.0);
}

void Idle() {
	if (animate) {
		rotx += 0.4;
	}
	glutPostRedisplay();
}

void Keyboard(unsigned char key, int x, int y) {
	switch(key) {
		case 'q' :
			exit(0);
			break;
		case 'a' :
			tx -= 0.5f;
			break;
		case 'd' :
			tx += 0.5f;
			break;
		default :
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
	// char path[] = "../planet.obj";
	ReadFile(&md, path);

	//Parameter handling
	glShadeModel (GL_SMOOTH);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  //renders a fragment if its z value is less or equal of the stored value
	glClearDepth(1);
    
	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial( GL_FRONT, GL_AMBIENT_AND_DIFFUSE );
  
	//Set up light source
	GLfloat light_position[] = { 0.0, 30.0, 50.0, 0.0 };
	glLightfv( GL_LIGHT0, GL_POSITION, light_position);

	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	// GLfloat specularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	   
	glLightfv( GL_LIGHT0, GL_AMBIENT, ambientLight );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	//01
	glFrontFace(GL_CCW);

	// Black background
	glClearColor(0.0f,0.0f,0.0f,1.0f);
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

void ReadFile(myModel *md, char *path) {
	string line;
	float x, y, z;
	string::size_type sz, sz2;
	ifstream myfile(path);
	if (myfile) {
		while (getline(myfile, line))	{
			if (line.find("v ") == 0) {
				string temp = line.substr(3);
				x = stod(temp, &sz);
				y = stod(temp.substr(sz), &sz2);
				z = stod(temp.substr(sz+sz2));
				md->v.push_back(new Point(x, y, z));
			} else if (line.find("vn ") == 0) {
				string temp = line.substr(3);
				x = stod(temp, &sz);
				y = stod(temp.substr(sz), &sz2);
				z = stod(temp.substr(sz+sz2));
				md->vn.push_back(new Point(x, y, z));
			} else if (line.find("f ") == 0) {
				int v1, vn1, v2, vn2, v3, vn3;
				string::size_type sz, sz2, sz3, sz4, sz5;
 				string temp = line.substr(3);
				v1 = stoi(temp, &sz);
				vn1 = stoi(temp.substr(sz+2), &sz2);
				v2 = stoi(temp.substr(sz+2+sz2), &sz3);
				vn2 = stoi(temp.substr(sz+2+sz2+sz3+2), &sz4);
				v3 = stoi(temp.substr(sz+2+sz2+sz3+2+sz4), &sz5);
				vn3 = stoi(temp.substr(sz+2+sz2+sz3+2+sz4+sz5+2));
				md->f.push_back(new Faces(v1, vn1, v2, vn2, v3, vn3));
			}
		}
		myfile.close();
	} else {
		cerr << "File " << path << " doesn't exist!" << endl;
		exit(EXIT_FAILURE);
	}
}

void DisplayModel(myModel md) {
	glPushMatrix();
	glBegin(GL_POINTS);

	for (size_t i = 0; i < md.f.size(); i++) {
		/* Vns */
		glVertex3f(md.vn[md.f[i]->getVn()->getX()-1]->getX(), md.vn[md.f[i]->getVn()->getX()-1]->getY(), md.vn[md.f[i]->getVn()->getX()-1]->getZ());
		glVertex3f(md.vn[md.f[i]->getVn()->getY()-1]->getX(), md.vn[md.f[i]->getVn()->getY()-1]->getY(), md.vn[md.f[i]->getVn()->getY()-1]->getZ());
		glVertex3f(md.vn[md.f[i]->getVn()->getZ()-1]->getX(), md.vn[md.f[i]->getVn()->getZ()-1]->getY(), md.vn[md.f[i]->getVn()->getZ()-1]->getZ());
		/* Vs */
		glVertex3f(md.v[md.f[i]->getV()->getX()-1]->getX(), md.v[md.f[i]->getV()->getX()-1]->getY(), md.v[md.f[i]->getV()->getX()-1]->getZ());
		glVertex3f(md.v[md.f[i]->getV()->getY()-1]->getX(), md.v[md.f[i]->getV()->getY()-1]->getY(), md.v[md.f[i]->getV()->getY()-1]->getZ());
		glVertex3f(md.v[md.f[i]->getV()->getZ()-1]->getX(), md.v[md.f[i]->getV()->getZ()-1]->getY(), md.v[md.f[i]->getV()->getZ()-1]->getZ());
	}

	glEnd();
	glPopMatrix();
}
