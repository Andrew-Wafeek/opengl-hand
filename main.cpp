#include <SDL2/SDL.h>
#include <GL\glut.h>
#include <cstdlib>
#include<iostream>
#include<string>
#include<stdlib.h>
#include"SerialPort.h"
using namespace std;
/*******************************************************/
#define LARGEUR_BASE 50
#define HAUTEUR_BASE 20
#define LARGEUR_BRAS_1 150
#define HAUTEUR_BRAS_1 15
#define LARGEUR_BRAS_2 50
#define HAUTEUR_BRAS_2 10
#define TAILLE_CAISSE 10
#define LARGEUR_ECRAN (LARGEUR_BASE + LARGEUR_BRAS_1 + HAUTEUR_BRAS_2 + 50)
#define HAUTEUR_ECRAN (HAUTEUR_BASE + LARGEUR_BRAS_1 + HAUTEUR_BRAS_2 + 50)
/*******************************************************/
char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];
char *port = "\\\\.\\COM7";
/*******************************************************/
int angle1 = 0;
int angle2 = -20;
int	angle3 = 10;
int longueur = 50;
int current_time = 0;
int deltaTime = 0;
int last_time = 0;
float anglePhalange1 = 0;	//> 0 < 90
float anglePhalange2 = 0;
float anglePhalange3 = 0;
float anglePhalange11 = 0;	//> 0 < 90
float anglePhalange22 = 0;
float anglePhalange33 = 0;
void Dessiner(SDL_Window*& window);
void delay(){
	for(int i=0;i<5000000;i++);
}
int main(int argc, char *argv[])
{
	SerialPort arduino(port);
	SDL_Event event;
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
	SDL_Window *window = SDL_CreateWindow("Hand Tracking", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 640, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
 	// Create an OpenGL context associated with the window.
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_ALPHA | GLUT_DEPTH|GLUT_STENCIL);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluPerspective(70,(double)LARGEUR_ECRAN/HAUTEUR_ECRAN, 1, 1000);
	//gluOrtho2D(0,LARGEUR_ECRAN,0,HAUTEUR_ECRAN);
	glEnable(GL_DEPTH_TEST);
 	Dessiner(window);
 	bool continued = true;
	while (continued)
	{
		arduino.readSerialPort(output, MAX_DATA_LENGTH);
		while(arduino.isConnected()){
			arduino.readSerialPort(output, MAX_DATA_LENGTH);
			switch((output[0])){
				case '0':
					anglePhalange1 = 0;
					anglePhalange2 = 0;
					anglePhalange3 = 0;
					anglePhalange11 = 0;
					anglePhalange22 = 0;
					anglePhalange33 = 0;
					break;
				case '1':
					anglePhalange1 = 7;
					anglePhalange2 = 15;
					anglePhalange3 = 30;
					break;
				case '2':
					anglePhalange1 = 30;
					anglePhalange2 = 60;
					anglePhalange3 = 90;
					break;
				case '3':
					anglePhalange1 = 70;
					anglePhalange2 = 85;
					anglePhalange3 = 90;
					break;
				case '4':
					anglePhalange11 = 7;
					anglePhalange22 = 15;
					anglePhalange33 = 30;
					break;
				case '5':
					anglePhalange11 = 30;
					anglePhalange22 = 60;
					anglePhalange33 = 90;
					break;
				case '6':
					anglePhalange11 = 70;
					anglePhalange22 = 85;
					anglePhalange33 = 90;
					break;
				case '#':
					anglePhalange1 = 0;
					anglePhalange2 = 0;
					anglePhalange3 = 0;
					break;
				case '*':
					anglePhalange11 = 0;
					anglePhalange22 = 0;
					anglePhalange33 = 0;
					break;
				default:
					break;
			}
			Dessiner(window);
			
		}
		
		current_time = SDL_GetTicks();
		deltaTime= current_time - last_time;
		last_time = current_time;
 		while(SDL_WaitEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
				exit(0);
				break;
				case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
 					case SDLK_q	:	if (anglePhalange1 < 90)			anglePhalange1 += 0.0045 * deltaTime;	break;
					case SDLK_e	:	if (anglePhalange2 < 90)			anglePhalange2 += 0.0045 * deltaTime;	break;
					case SDLK_t	:	if (anglePhalange3 < 90)			anglePhalange3 += 0.0045 * deltaTime;	break;
 					case SDLK_w	:	if (anglePhalange1 > 0)				anglePhalange1 -= 0.0045 * deltaTime;	break;
					case SDLK_r	:	if (anglePhalange2 > 0)				anglePhalange2 -= 0.0045 * deltaTime;	break;
					case SDLK_y	:	if (anglePhalange3 > 0)				anglePhalange3 -= 0.0045 * deltaTime;	break;
					case SDLK_a	:	if (anglePhalange11 < 90)			anglePhalange11 += 0.0045 * deltaTime;	break;
					case SDLK_d	:	if (anglePhalange22 < 90)			anglePhalange22 += 0.0045 * deltaTime;	break;
					case SDLK_g	:	if (anglePhalange33 < 90)			anglePhalange33 += 0.0045 * deltaTime;	break;
 					case SDLK_s	:	if (anglePhalange11 > 0)				anglePhalange11 -= 0.0045 * deltaTime;	break;
					case SDLK_f	:	if (anglePhalange22 > 0)				anglePhalange22 -= 0.0045 * deltaTime;	break;
					case SDLK_h	:	if (anglePhalange33 > 0)				anglePhalange33 -= 0.0045 * deltaTime;	break;
				}
			}
			Dessiner(window);
		}
	}
	return 0;
}
void createCube(float scale = 1.0f, float scaleX = 1.0f, float scaleY = 1.0f, float scaleZ = 1.0f)	//X = width, Y = length, Z = height
{
	glBegin(GL_QUADS);
		glColor3ub(255, 0, 0); //face rouge
		glVertex3d(scaleX * scale  , scaleY * scale  , scaleZ * scale);
		glVertex3d(scaleX * scale  , scaleY * scale  , -scaleZ * scale);
		glVertex3d(-scaleX * scale  , scaleY * scale  , -scaleZ * scale);
		glVertex3d(-scaleX * scale  , scaleY * scale  , scaleZ * scale);
 		glColor3ub(0, 255, 0); //face verte
		glVertex3d(scaleX * scale  , -scaleY * scale  , scaleZ * scale);
		glVertex3d(scaleX * scale  , -scaleY * scale  , -scaleZ * scale);
		glVertex3d(scaleX * scale  , scaleY * scale  , -scaleZ * scale);
		glVertex3d(scaleX * scale  , scaleY * scale  , scaleZ * scale);
 		glColor3ub(0, 0, 255); //face bleue
		glVertex3d(-scaleX * scale  , -scaleY * scale  , scaleZ * scale);
		glVertex3d(-scaleX * scale  , -scaleY * scale  , -scaleZ * scale);
		glVertex3d(scaleX * scale  , -scaleY * scale  , -scaleZ * scale);
		glVertex3d(scaleX * scale  , -scaleY * scale  , scaleZ * scale);
 		glColor3ub(255, 255, 0); //face jaune
		glVertex3d(-scaleX * scale  , scaleY * scale  , scaleZ * scale);
		glVertex3d(-scaleX * scale  , scaleY * scale  , -scaleZ * scale);
		glVertex3d(-scaleX * scale  , -scaleY * scale  , -scaleZ * scale);
		glVertex3d(-scaleX * scale  , -scaleY * scale  , scaleZ * scale);
 		glColor3ub(0, 255, 255); //face cyan
		glVertex3d(scaleX * scale  , scaleY * scale  , -scaleZ * scale);
		glVertex3d(scaleX * scale  , -scaleY * scale  , -scaleZ * scale);
		glVertex3d(-scaleX * scale  , -scaleY * scale  , -scaleZ * scale);
		glVertex3d(-scaleX * scale  , scaleY * scale  , -scaleZ * scale);
 		glColor3ub(255, 0, 255); //face magenta
		glVertex3d(scaleX * scale  , -scaleY * scale  , scaleZ * scale);
		glVertex3d(scaleX * scale  , scaleY * scale  , scaleZ * scale);
		glVertex3d(-scaleX * scale  , scaleY * scale  , scaleZ * scale);
		glVertex3d(-scaleX * scale  , -scaleY * scale  , scaleZ * scale);
	glEnd();
}
 void Dessiner(SDL_Window*& window)
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
 	gluLookAt(3, -5, 2,1,0,0,0,0,1);
 	glPushMatrix();
	
	//tronc
	createCube(1, 0.7, 0.3);
	/////////////////Finger1////////////////////
	glTranslatef(0.7 - 0.13, 0, 1.3);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	createCube(1, 0.13, 0.2, 0.3);
	glPopMatrix(); // get camera matrix
	glPushMatrix(); // save camera matrix
	/////////////////Finger2////////////////////
	glTranslatef(0.7 - 0.42, 0, 1.3);
	glRotatef(anglePhalange1, 1, 0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	glRotatef(anglePhalange2, 1, 0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	glRotatef(anglePhalange3, 1, 0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glPopMatrix(); // get camera matrix
	glPushMatrix(); // save camera matrix
	/////////////////Finger3////////////////////
	glTranslatef(0, 0, 1.3);
	glRotatef(anglePhalange11, 1, 0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	glRotatef(anglePhalange22, 1, 0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	glRotatef(anglePhalange33, 1,0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glPopMatrix(); // get camera matrix
	glPushMatrix(); // save camera matrix
	/////////////////Finger4////////////////////
	glTranslatef(-0.299, 0, 1.3);
	//glRotatef(anglePhalange3, 1, 0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	createCube(1, 0.13, 0.2, 0.3);
	glPopMatrix(); // get camera matrix
	glPushMatrix(); // save camera matrix
	/////////////////Finger5////////////////////
	glTranslatef(-0.6, 0, 1.3);
	//glRotatef(anglePhalange3, 1, 0, 0);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	createCube(1, 0.13, 0.2, 0.3);
	glTranslatef(0, 0, 0.6);
	createCube(1, 0.13, 0.2, 0.3);
	glPopMatrix(); // get camera matrix
	glPushMatrix(); // save camera matrix
 	glFlush();
	SDL_GL_SwapWindow(window);
}
