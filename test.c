#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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

int xsz, ysz;
float angle;
int objsel;

int main(int argc, char **argv)
{
	float lpos[] = {-1, 1, 1, 0};
	float white[] = {1, 1, 1, 1};
	float color[] = {0.9, 0.8, 0.73, 1};

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 60.0);

	glutMainLoop();
	return 0;
}

void disp(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(200, 0, xsz - 200, ysz);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)(xsz - 200) / (float)ysz, 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -8);
	glRotatef(25, 1, 0, 0);
	glRotatef(angle, 0, 1, 0);

	switch(objsel) {
	case 0:
		glFrontFace(GL_CW);
		glutSolidTeapot(1.0);
		glFrontFace(GL_CCW);
		break;

	case 1:
		glutSolidTorus(0.5, 1, 12, 24);
		break;

	case 2:
		glutSolidSphere(1.0, 24, 12);
		break;

	default:
		break;
	}


	glViewport(0, 0, 200, ysz);
	imtk_set_viewport(200, ysz);

	gui();

	glutSwapBuffers();
	assert(glGetError() == GL_NO_ERROR);
}

void gui(void)
{
	static int bnshow;
	static char textbuf[256];
	static char textbuf2[256];
	static float val;
	/*static int prev_sel;
	char *itemlist;*/

	imtk_begin();

	glBegin(GL_QUADS);
	glColor3f(0.6, 0.6, 0.6);
	glVertex2f(0, 0);
	glVertex2f(200, 0);
	glVertex2f(200, glutGet(GLUT_WINDOW_HEIGHT));
	glVertex2f(0, glutGet(GLUT_WINDOW_HEIGHT));
	glEnd();

	if(imtk_button(IMUID, "red", 30, 50)) {
		float color[] = {1, 0.4, 0.3, 1};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
		glutPostRedisplay();
	}
	if(imtk_button(IMUID, "blue", 30, 80)) {
		float color[] = {0.3, 0.4, 1, 1};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
		glutPostRedisplay();
	}

	/*itemlist = imtk_create_list("teapot", "torus", "sphere", NULL);
	if((objsel = imtk_listbox(IMUID, itemlist, prev_sel, 30, 120)) != prev_sel) {
		prev_sel = objsel;
		glutPostRedisplay();
	}
	imtk_free_list(itemlist);*/

	imtk_textbox(IMUID, textbuf, sizeof textbuf, 30, 200);
	imtk_textbox(IMUID, textbuf2, sizeof textbuf2, 30, 250);

	if((bnshow = imtk_checkbox(IMUID, "show hidden button", 30, 300, bnshow))) {
		if(imtk_button(IMUID, "yellow", 50, 340)) {
			float color[] = {0.8, 0.75, 0.3, 1};
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, color);
			glutPostRedisplay();
		}
	}

	val = imtk_slider(IMUID, angle, 0.0, 360.0, 30, 390);
	if(val != angle) {
		angle = val;
		glutPostRedisplay();
	}

	imtk_progress(IMUID, val / 360.0, 30, 420);

	if(imtk_button(IMUID, "Quit", 30, 500)) {
		exit(0);
	}

	imtk_end();
}

void reshape(int x, int y)
{
	xsz = x;
	ysz = y;

	glViewport(0, 0, x, y);
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
