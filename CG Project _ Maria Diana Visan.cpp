/* ===================================================================================
	Departamento Eng. Informatica - FCTUC
	Project Computacao Grafica - 2021/22

	................................................
	Student name: Maria Diana Visan
	Number: 2021160418
======================================================================================= */

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <glut.h>

#include "RgbImage.h"


//--------------------------------- Define colors
#define BLUE     0.0, 0.0, 1.0, 1.0
#define RED		 1.0, 0.0, 0.0, 1.0
#define YELLOW	 1.0, 1.0, 0.0, 1.0
#define GREEN    0.0, 1.0, 0.0, 1.0
#define WHITE    1.0, 1.0, 1.0, 1.0
#define BLACK    0.0, 0.0, 0.0, 1.0
#define GRAY     0.3, 0.3, 0.3, 1.0
#define PI		 3.14159


void initMaterials(int material);

char Materiais[5][30] = { "CYAN",  "GREEN",  "RED",  "WHITE", "YELLOW" };


GLfloat alpha = 0.75;
GLfloat tam = 0.5;
char text[30];
GLint material = 10;


//=========================================================== Components of joystick  (button & handle)

GLfloat rotation_handle = -90.0;

GLfloat trans_ball_z = 0;

GLfloat trans_ball_y = 3 * tam;

GLfloat trans_buttonLeft_y = -0.25; //left button translation on Y axis

GLfloat trans_buttonRight_y = -0.25; //right button translation on Y axis

//=========================================================== TABLE FACES


static GLuint    left[] = { 0, 1, 2, 3 }; //left
static GLuint     up[] = { 8, 11, 10,  9 };   //up = right hand rule
static GLuint     right[] = { 4, 7, 6, 5 }; //right
static GLuint     down[] = { 12, 13, 14, 15 }; //down
static GLuint     front[] = { 16,18,19,17 }; //front
static GLuint     back[] = { 20,21,22,23 }; //back


//------------------------------------------------------------ Objects (coordinate system) right
GLint		wScreen = 800, hScreen = 600;			//... window (pixels)
GLfloat		xC = 10.0, yC = 10.0, zC = 10.0;		//Global coordinates



//------------------------------------------------------------ Visualizacao/Observador 
GLfloat  rVision = 10, aVision = 0.5 * PI, incVision = 0.05;
GLfloat  obsP[] = { rVision * cos(aVision), 3.0, rVision * sin(aVision) };
GLfloat  obsT[] = { obsP[0] - rVision * cos(aVision), obsP[1], obsP[2] - rVision * sin(aVision) };
GLfloat  angZoom = 45;
GLfloat  incZoom = 3;

//---------------------------------------------------- Spot light on the CEILING (Y axis)
GLint   turnOnSpotlight = 2;		 //:::   'T'  
GLfloat intensitySpotlight = 0.8;  //:::   'I'  
GLint   lightR = 0.5;		 	 //:::   'R'  
GLint   lightG = 1;			 //:::   'G'  
GLint   lightB = 1;			 //:::   'B'  
GLfloat localPos[4] = { 0.0, 5.0, 0.0, 1.0 };
GLfloat localColorAmb[4] = { 0, 0, 0, 0.0 };
GLfloat localColorDif[4] = { lightR, lightG, lightB, 1.0 };
GLfloat localColorEsp[4] = { lightR, lightG, lightB, 1.0 };
float atConst = 1.0;


GLfloat switchFocus = 10.0; //intial angle of the focus light source
GLfloat Pos[] = { -1.0f, 0.0f,  2.0f, 1.0f }; //Focus light position
GLfloat		angleINC = 3.0;		//.. incrementation of light angle
GLfloat		angleMIN = 5.0;		//.. mminimum valuem of light angle
GLfloat		angleMAX = 100.0;		//.. maximum value of light angle


void defineLights() {

	GLfloat Focus_direction[] = { 0, 0, -1, 0 };	//……… -Z
	GLfloat Focus_color[] = { 0, 1,  0, 1 };	//……… Cor da luz 1
	GLfloat Focus_k = 1.0;
	GLfloat Focus_l = 0.05f;
	GLfloat Focus_q = 0.0f;
	GLfloat Focus_exponent = 2.0;		// Foco, SPOT_Exponent


	//……………………………………………………………………………………………………………………………Focus Light
	glLightfv(GL_LIGHT0, GL_POSITION, Pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Focus_color);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Focus_k);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Focus_l);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Focus_q);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, switchFocus);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Focus_direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, Focus_exponent);

	//……………………………………………………………………………………………………………………………SpotLight
	glLightfv(GL_LIGHT1, GL_POSITION, localPos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, localColorAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, localColorDif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, localColorEsp);




}

//================================================================================
//===========================================================Variables and constants


static GLfloat vertices[] = {
	//…………………………………………………………………………………………………… x=tam (Left)
		-tam,  -tam,  tam,	// 0 
		-tam,   tam,  tam,	// 1 
		-tam,   tam, -tam,	// 2 
		-tam,  -tam, -tam,	// 3 
	//…………………………………………………… Right
		 tam,  -tam,  tam,	// 4 
		 tam,   tam,  tam,	// 5 
		 tam,   tam, -tam,	// 6 
		 tam,  -tam, -tam,	// 7 
	//……………………………………………………… Up
		-tam,  tam,  tam,	// 8 
		-tam,  tam, -tam,	// 9 
		 tam,  tam, -tam,	// 10 
		 tam,  tam,  tam,	// 11
		 //……………………………………………………… Down
		-tam,  -tam,  tam,	// 12 
		-tam,  -tam, -tam,	// 13
		tam,  -tam, -tam,	// 14 
		 tam,  -tam, tam,	// 15 

		//……………………………………………………… Front
		-tam,  tam,  tam,	// 16 
		tam,  tam, tam,	// 17
		-tam,  -tam, tam,	// 18 
		 tam,  -tam, tam,	// 19

		 //……………………………………………………… Back
		-tam,  tam,  -tam,	// 20 
		tam,  tam, -tam,	// 21
		tam,  -tam, -tam,	// 22 
		 -tam,  -tam, -tam,	// 23

};

static GLfloat normals[] = {
	//…………………………………………………………………………………………………… x=tam (Left)
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  -1.0,  0.0,  0.0,
	  //…………………………………………………………………………………………………… x=tam (Right)
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		1.0,  0.0,  0.0,
		//…………………………………………………………………………………………………… y=tam (Up)
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,

	   //…………………………………………………………………………………………………… y = tam (Down)
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,
	   0.0,  -1.0,  0.0,

	   //…………………………………………………………………………………………………… z = tam (Front)
	   0.0, 0.0,  1.0,
	   0.0,  0.0,  1.0,
	   0.0,  0.0,  1.0,
	   0.0,  0.0,  1.0,

	   //…………………………………………………………………………………………………… z = tam (Back)
	   0.0, 0.0,  -1.0,
	   0.0,  0.0,  -1.0,
	   0.0,  0.0,  -1.0,
	   0.0,  0.0,  -1.0,

};

//------------------------------------------------------------ Colors array
static GLfloat colors[] = {

	0.6, 0.0, 0.0,
  0.6, 0.0, 0.0,
 0.6, 0.0, 0.0,
0.6, 0.0, 0.0,

	0.6, 0.0, 1.0,
  0.6, 0.0, 1.0,
 0.6, 0.0, 1.0,
0.6, 0.0, 1.0,

   0.6, 0.0, 0.9,
  0.6, 0.0, 0.9,
 0.6, 0.0, 0.9,
0.6, 0.0, 0.9,

  0.6, 0.0, 0.0,
  0.6, 0.0, 0.0,
 0.6, 0.0, 0.0,
0.6, 0.0, 0.0,

	0.6, 0.0, 1.0,
  0.6, 0.0, 1.0,
 0.6, 0.0, 1.0,
0.6, 0.0, 1.0,

   0.6, 0.0, 0.9,
  0.6, 0.0, 0.9,
 0.6, 0.0, 0.9,
0.6, 0.0, 0.9,


};

static GLfloat textures[] = {
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
2, 2,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1,
0, 0,
1, 0,
1, 1,
0, 1 };
//---------------------------------------------------------------------------Textures


RgbImage imag;

GLuint   texture[1];


void initTextures() {

	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("texture.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());


}

//================================================================================
//=========================================================================== INIT
void init(void)
{
	glClearColor(BLACK);		//………………………………………………………………………………Delete
	glEnable(GL_DEPTH_TEST);	//………………………………………………………………………………Depth
	glShadeModel(GL_SMOOTH);	//………………………………………………………………………………Color interpolaton	

	//------ Texture

	initTextures();

	glVertexPointer(3, GL_FLOAT, 0, vertices); //………………………………………VertexArrays: vertices + normals + colors
	glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer(GL_FLOAT, 0, normals);
	glEnableClientState(GL_NORMAL_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, colors);
	glEnableClientState(GL_COLOR_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, textures);   // coordenadas textura
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);



	//--------------------------------------------------- Light activation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	defineLights();

	initMaterials(10); //gold


}

//================================================================================
//======================================================================== DISPLAY

void drawText(char* string, GLfloat x, GLfloat y, GLfloat z) {
	glRasterPos3f(x, y, z);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *string++);
}
GLvoid resize(GLsizei width, GLsizei height) {
	wScreen = width;
	hScreen = height;
	glViewport(0, 0, wScreen, hScreen);
	glutPostRedisplay();
}

void drawAxes()
{
	//~~~~~~~~~~~~~~~~~ X
	glColor4f(BLUE);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~ Y
	glColor4f(GREEN);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	//~~~~~~~~~~~~~~~~~ Z
	glColor4f(RED);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();

}





void drawScene() {
	initMaterials(material);

	//------------------------------------ LIGHTING

	glEnable(GL_LIGHTING);

	defineLights();


	//-------------------------BASE CUBE
	glPushMatrix(); //GLOBAL PUSH






	glPushMatrix(); //PUSH CUBE & THE REST

	glScalef(3, 1, 3);

	//------------------------------------------------------- Top of the object

	glPushMatrix(); //PUSH UP FACE

	glTranslatef(0, -(1.75 * tam), 0);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, up);

	glPopMatrix(); //POP UP FACE


	//------------------------------------------------------- Texture and side faces of the base
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); //PUSH RIGHT FACE
	glTranslatef(0, -(1.75 * tam), 0);
	//glScalef(1, 2, 2);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, right);
	glPopMatrix(); //POP RIGHT FACE

	glPushMatrix(); //PUSH LEFT FACE
	glTranslatef(0, -(1.75 * tam), 0);
	//glScalef(1, 2, 2);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, left);
	glPopMatrix();//POP LEFT FACE
	glPushMatrix(); //PUSH FRONT FACE

			//glTranslatef(2, 1, 4);
			//glScalef(1,1,5);

	glTranslatef(0, -(1.75 * tam), 0);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, front);

	glPopMatrix(); //PUSH FRONT FACE



	glPushMatrix(); //PUSH BACK FACE

	glTranslatef(0, -(1.75 * tam), 0);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, back);

	glPopMatrix(); //POP BACK FACE


	glDisable(GL_TEXTURE_2D);

	//------------------------------------------------------- Base of the object
	glPushMatrix(); //PUSH DOWN FACE


	glTranslatef(0, -(1.75 * tam), 0);

	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, down);

	glPopMatrix(); //POP DOWN FACE





	glPopMatrix();

	glPopMatrix(); //GLOBAL POP





	//....



	glPushMatrix(); //PUSH SPHERE

	glColor3f(1, 0, 0);

	glTranslatef(0, trans_ball_y, trans_ball_z);
	glutSolidSphere(0.5, 20, 20); //draw  sphere


	glPopMatrix(); //POP SPHERE


	glPushMatrix(); //PUSH HANDLE


	glColor3f(0.6, 0.5, 0.9);
	glTranslatef(0, -1 * tam, 0);
	glRotatef(rotation_handle, 1, 0, 0);

	glScalef(0.8, 0.8, 1.0);
	glutSolidCone(1, 2, 50, 50);


	glPopMatrix(); //POP HANDLE


	glPushMatrix(); //PUSH RING

	glColor3f(0.9, 0.9, 1.0);
	glRotatef(240, 1, 1, 1);
	glTranslatef(0., 0., -0.3);

	glutSolidTorus(0.1f, 0.8f, 32, 32);


	glPopMatrix(); //POP RING

	//========================================================================  BUTTONS (transparent)


	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix(); //PUSH BUTTON LEFT

	glColor3f(1.0, 0.9, 0.1);
	//glRotated(90, 1, 0, 0);
	glTranslatef(1, trans_buttonLeft_y, 1);
	glScalef(0.5, 0.2, 0.5);

	glutSolidCube(1.5);


	glPopMatrix(); //POP BUTTON LEFT
	glDisable(GL_BLEND);


	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix(); //PUSH BUTTON RIGHT

	glColor3f(1.0, 0.9, 0.1);
	//glRotated(90, 1, 0, 0);
	glTranslatef(-1, trans_buttonRight_y, 1);
	glScalef(0.5, 0.2, 0.5);

	glutSolidCube(1.5);


	glPopMatrix(); //POP BUTTON RIGHT

	glDisable(GL_BLEND);



}


void display(void) {

	glClearColor(0, 0, 0, 1.0);

	//================================================================= Erases screen and reads in depth (3D)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//================================================================= Viewport1
	glViewport(0, 0, 0.3 * wScreen, 0.3 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-xC, xC, -yC, yC, -zC, zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 5, 0, 0, 0, 0, 0, 0, -1);

	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);


	glColor3f(1, 1, 1);
	

	//…………………………………………………………………………………………………………………………………………………………Objects calls
	drawAxes();
	drawScene();


	//. . . . . . . . . . . . . . . . . . . . .  Actualisation
	glutSwapBuffers();
}
//------------------------------------------------ Display Information for  User



//======================================================== LIGHT UPDATE for INTENSITY CONTROL

void updateLight() {
	localColorAmb[0] = lightR * intensitySpotlight;
	localColorAmb[1] = lightG * intensitySpotlight;
	localColorAmb[2] = lightB * intensitySpotlight;
	localColorDif[0] = lightR * intensitySpotlight;
	localColorDif[1] = lightG * intensitySpotlight;
	localColorDif[2] = lightB * intensitySpotlight;
	localColorEsp[0] = lightR * intensitySpotlight;
	localColorEsp[1] = lightG * intensitySpotlight;
	localColorEsp[2] = lightB * intensitySpotlight;;
	glLightfv(GL_LIGHT0, GL_AMBIENT, localColorAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, localColorDif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, localColorEsp);
}
//======================================================= KEYBOARD EVENTS (ANIMATION)


void keyboard(unsigned char key, int x, int y) {

	switch (key) {
		//--------------------------- Full screen control
	case 'f': case 'F':
		glutFullScreen();
		glutPostRedisplay();
		break;


		//--------------------------- Left side handle animation
	case 'a': case 'A':
		if (rotation_handle <= -90.) {

			rotation_handle += 10.00;

		}

		if (trans_ball_z <= 0.2) {
			trans_ball_z += 0.3;
			//trans_ball_y -= 0.1;
		}

		glutPostRedisplay();
		break;

		//--------------------------- Right side handle animation



	case 's': case 'S':
		if (rotation_handle >= -90.) {
			rotation_handle -= 10.00;
		}
		if (trans_ball_z >= -0.2) {
			trans_ball_z -= 0.3;
			//trans_ball_y -= 0.1;
		}
		glutPostRedisplay();
		break;
		//---------------------------Left Button animation
	case 'q': case 'Q':
		trans_buttonLeft_y = -0.4;
		glutPostRedisplay();
		break;

	case 'w': case 'W':
		trans_buttonLeft_y = -0.25;
		glutPostRedisplay();
		break;
		//---------------------------Right Button animation
	case 'e': case 'E':
		trans_buttonRight_y = -0.4;

		glutPostRedisplay();
		break;

	case 'r': case 'R':
		trans_buttonRight_y = -0.25;

		glutPostRedisplay();
		break;
		//===================================== ILUMINNATION

		//--------------------------- SPOTLIGHT ON/OFF
	case 't':case 'T':
		turnOnSpotlight = !turnOnSpotlight;
		turnOnSpotlight = !turnOnSpotlight;
		updateLight();
		glutPostRedisplay();
		break;
		//---------------------------Spotlight Intensity
	case 'i': case 'I':
		intensitySpotlight = intensitySpotlight + 0.1;
		if (intensitySpotlight > 1.8) intensitySpotlight = 0;
		updateLight();
		glutPostRedisplay();
		break;
		//--------------------------- Materials
	case 'm': case 'M':
		material = (material + 1) % 5;
		initMaterials(material);

		glutPostRedisplay();
		break;


		//--------------------------- Escape
	case 27:
		exit(0);
		break;
	}






}



void teclasNotAscii(int key, int x, int y) {
	//... observer can walk around the scene (left/right arrows)
	//... observer can walk up and down (up/down arrows)

	obsT[0] = 0; obsT[1] = 0; obsT[2] = 0;

	if (key == GLUT_KEY_UP) {

		obsP[1] = obsP[1] + 0.5;

	}
	if (key == GLUT_KEY_DOWN) {

		obsP[1] = obsP[1] - 0.5;
	}

	if (key == GLUT_KEY_LEFT) {
		aVision = aVision + 0.1;
	}
	if (key == GLUT_KEY_RIGHT) {
		aVision = aVision - 0.1;
	}


	if (obsP[1] > yC) {
		obsP[1] = yC;
	}

	if (obsP[1] < -yC) {
		obsP[1] = -yC;
	}
	obsP[0] = rVision * cos(aVision);
	obsP[2] = rVision * sin(aVision);

	printf(" %f ", obsP[1]);



	if (key == GLUT_KEY_F1) {
		switchFocus = switchFocus + angleINC;
		if (switchFocus > angleMAX)
			switchFocus = angleMAX;
	}
	if (key == GLUT_KEY_F2) {
		switchFocus = switchFocus - angleINC;
		if (switchFocus < angleMIN)
			switchFocus = angleMIN;
	}
	glutPostRedisplay();



}


//======================================================= MAIN FUNCTION

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("|Maria Diana Visan 2021160418|  |uc2021160418@student.uc.pt| |Project Compiter Graphics");



	init();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}