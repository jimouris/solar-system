#include <stdio.h>     // - Just for some ASCII messages
#include <string.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <vector>
#include <string>

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

Point::Point(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Faces::Faces(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
	this->p1 = new Point(x1, y1, z1);
	this->p2 = new Point(x2, y2, z2);
	this->p3 = new Point(x3, y3, z3);
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

	glutSwapBuffers();	// All drawing commands applied to the hidden buffer, so now, bring forward the hidden buffer and hide the visible one
}

//-----------------------------------------------------------

void Resize(int w, int h) { 
  // define the visible area of the window ( in pixels )
  if (h==0) h=1;
  glViewport(0,0,w,h); 

  // Setup viewing volume
  glMatrixMode(GL_PROJECTION); 
  glLoadIdentity();
 
  gluPerspective(60.0, (float)w/(float)h, 1.0, 500.0);
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
			tx-=0.5f;
			break;
		case 'd' :
			tx+=0.5f;
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


void Setup() { 
	char path[] = "../planet.obj";
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

/*
bool loadOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals) {
	while(1) {
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if ( strcmp(lineHeader, "v") == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		} else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
}
*/

void ReadFile(model *md, char *path) {

	string line;
	ifstream myfile(path);
	if (myfile) {
		while (getline(myfile, line))	{
			if (line.find("v ") == 0) {
				// istringstream iss(line);
			 //    vector<string> tokens;
			 //    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));

				float x,y,z;
				string::size_type sz,sz2;


				string temp=line.substr(3);
				x=stod(temp,&sz);
				y=stod(temp.substr(sz),&sz2);
				z=stod(temp.substr(sz+sz2));
				//cout << temp << endl;
				cout << x << endl;
				cout << y << endl;
				cout << z << endl;

				//cout << line;
				exit(1);
			} else if (line.find("vn ") == 0) {
			    // std::cout << "String starts with vn\n";


			} else if (line.find("f ") == 0) {
			    // std::cout << "String starts with f\n";

			}
		}
		myfile.close();
	}

	/*
	FILE *fp = fopen(path, "r");

	if (fp == NULL) {
		fprintf(stderr, "Error openining %s!\n", path);
		exit(EXIT_FAILURE);
	}
	while (1) {
		char lineHeader[128];
		int res = fscanf(fp, "%s", lineHeader);
		if (res == EOF) {
			fprintf(stderr, "I found eof\n");
			break;
		}
		if (!strcmp(lineHeader, "v")) {
			// fprintf(stderr, "I found v\n");
			float x=0, y=0, z=0;
			fp >> x >> y >> z;

			// Point *p = new Point(x, y, z);
			
		} else if (!strcmp(lineHeader, "vn" )) {
			fprintf(stderr, "I found vn\n");

		} else if (!strcmp(lineHeader, "f" )) {
			fprintf(stderr, "I found f\n");

		}
	}
	fclose(fp);
*/

	/*
	// ifstream obj_file("OBJINFO.TXT");                   // Open the file for reading OBJINFO.TXT

	// if (obj_file.fail()) {
	// 	exit(EXIT_FAILURE);
	// }
	// cout << "adjsahdjash "<< md->vertices << endl;
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
	*/
}

void DisplayModel(model md) {
	glPushMatrix();
	glBegin(GL_TRIANGLES);

	for (int i = 0; i < md.faces; i++) {
		// glVertex3f(md.obj_points[md.obj_faces[i].vtx[0]-1].x,md.obj_points[md.obj_faces[i].vtx[0]-1].y,md.obj_points[md.obj_faces[i].vtx[0]-1].z);
		// glVertex3f(md.obj_points[md.obj_faces[i].vtx[1]-1].x,md.obj_points[md.obj_faces[i].vtx[1]-1].y,md.obj_points[md.obj_faces[i].vtx[1]-1].z);
		// glVertex3f(md.obj_points[md.obj_faces[i].vtx[2]-1].x,md.obj_points[md.obj_faces[i].vtx[2]-1].y,md.obj_points[md.obj_faces[i].vtx[2]-1].z);
	}

	glEnd();
	glPopMatrix();

}
