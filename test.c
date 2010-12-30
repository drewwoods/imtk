#include <stdio.h>
#include <stdlib.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "imtk.h"

void disp(void);
void gui(void);
void reshape(int x, int y);
void keyb(unsigned char key, int x, int y);
void keyb_up(unsigned char key, int x, int y);
void skeyb(int key, int x, int y);
void skeyb_up(int key, int x, int y);
void mouse(int bn, int state, int x, int y);
void motion(int x, int y);


int main(int argc, char **argv)
{
	glutInitWindowSize(800, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("imgui test");

	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);
	glutKeyboardUpFunc(keyb_up);
	glutSpecialFunc(skeyb);
	glutSpecialUpFunc(skeyb_up);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(motion);

	glutMainLoop();
	return 0;
}

void disp(void)
{
	glClearColor(0.6, 0.6, 0.6, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gui();

	glutSwapBuffers();
}

void gui(void)
{
	static int bnshow;

	imtk_begin();

	if(imtk_button(IMUID, "foobar", 100, 100)) {
		printf("clicked button 0\n");
	}
	if(imtk_button(IMUID, "xyzzy", 100, 200)) {
		printf("clicked button 1\n");
	}
	if(imtk_button(IMUID, "Quit", 100, 500)) {
		exit(0);
	}

	if((bnshow = imtk_checkbox(IMUID, "show hidden button", 100, 260, bnshow))) {
		if(imtk_button(IMUID, "I was hidden!", 130, 300)) {
			printf("you clicked the hidden button!\n");
		}
	}

	imtk_end();
}

void reshape(int x, int y)
{
	glViewport(0, 0, x, y);
	imtk_inp_reshape(x, y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glTranslatef(-1, -1, 0);
	glScalef(2.0 / x, 2.0 / y, 1.0);
}

void keyb(unsigned char key, int x, int y)
{
	switch(key) {
	case 27:
		exit(0);

	default:
		break;
	}

	imtk_inp_key(key, IMTK_DOWN);
}

void keyb_up(unsigned char key, int x, int y)
{
	imtk_inp_key(key, IMTK_UP);
}

void skeyb(int key, int x, int y)
{
	imtk_inp_key(key, IMTK_DOWN);
}

void skeyb_up(int key, int x, int y)
{
	imtk_inp_key(key, IMTK_UP);
}

void mouse(int bn, int state, int x, int y)
{
	imtk_inp_mouse(bn, state == GLUT_DOWN ? IMTK_DOWN : IMTK_UP);
}

void motion(int x, int y)
{
	imtk_inp_motion(x, y);
}
