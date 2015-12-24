#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "GL/glut.h"   // - An interface and windows management library
#include "visuals.h"   // Header file for our OpenGL functions

model md;
static float tx = 0.0;
static float rotx = 0.0;
static bool animate = false;
static float red = 1.0;
static float green = 0.0;
static float blue = 0.0;

using namespace std;

void keimeno(const char *str,float size)
{

	glPushMatrix();
	glScalef(size,size,size);

	for (size_t i=0;i<strlen(str);i++)
	  glutStrokeCharacter(GLUT_STROKE_ROMAN,str[i]);
	glPopMatrix();

}

void Render()
{    
  //CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
													   // and the depth buffer
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity();
	
 
  glTranslatef(0,0,-100);
  glTranslatef(tx,0.0,0.0);
  glRotatef(rotx,1,0,0);

  
 
  //(01)             
  glColor3f(0.3, 0.2, 0.9);                            // Set drawing colour
  DisplayModel(md);

  //(02)
  //glColor3f(0.8, 0.1, 0.1);
  //glTranslatef(-20.0,0.0,0.0);
  //keimeno("Dokimastiko keimeno",0.05f);

  //(03)             
  //glColor3f(red, green, blue);                            // Set drawing colour
  //glutSolidTeapot(20.0);
  
 
  glutSwapBuffers();             // All drawing commands applied to the 
                                 // hidden buffer, so now, bring forward
                                 // the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Resize(int w, int h)
{ 
  // define the visible area of the window ( in pixels )
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  // Setup viewing volume

  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
  gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
}

void Idle()
{
	if(animate)
		rotx+=0.4;

	glutPostRedisplay();
}

void Keyboard(unsigned char key,int x,int y)
{
	switch(key)
	{
	case 'q' : exit(0);
		break;
	case 'a' : tx-=0.5f;
		break;
	case 'd' : tx+=0.5f;
		break;
	default : break;
	}

	glutPostRedisplay();

}

void Mouse(int button,int state,int x,int y)
{
	 if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
	 {
		 animate = !animate;
         glutPostRedisplay();
	 }
}


void Setup()  // TOUCH IT !! 
{ 
	ReadFile(&md);

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

void MenuSelect(int choice)
{
	switch (choice) {
		case RED : 
			red = 1.0; 
			green = 0.0; 
			blue = 0.0; break;
		case GREEN : 
			red = 0.0; 
			green = 1.0; 
			blue = 0.0; break;
		case BLUE : 
			red = 0.0; 
			green = 0.0; 
			blue = 1.0; break;
		case WHITE : 
			red = 1.0; 
			green = 1.0; 
			blue = 1.0; break;
	}

}

void ReadFile(model *md)
{

	ifstream obj_file("OBJINFO.TXT");                   // Open the file for reading OBJINFO.TXT
   
	if (obj_file.fail()) 
		exit(1);

	obj_file >> md->vertices;                               // Get the number of vertices
	obj_file >> md->faces;									// Get the number of faces

	for (int i = 0; i < md->vertices; i++){                        // Get the vertex coordinates
	   
		obj_file >> md->obj_points[i].x;
		obj_file >> md->obj_points[i].y;
		obj_file >> md->obj_points[i].z;
	
   }

   for (int i = 0; i < md->faces; i++){                        // Get the face structure
	   
		obj_file >> md->obj_faces[i].vtx[0];
		obj_file >> md->obj_faces[i].vtx[1];
		obj_file >> md->obj_faces[i].vtx[2];
	
   }

   obj_file.close();
}

void DisplayModel(model md)
{

	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++)
	{
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x,md.obj_points[md.obj_faces[i].vtx[0]-1].y,md.obj_points[md.obj_faces[i].vtx[0]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x,md.obj_points[md.obj_faces[i].vtx[1]-1].y,md.obj_points[md.obj_faces[i].vtx[1]-1].z);
		glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x,md.obj_points[md.obj_faces[i].vtx[2]-1].y,md.obj_points[md.obj_faces[i].vtx[2]-1].z);
	}

	glEnd();
	glPopMatrix();

}