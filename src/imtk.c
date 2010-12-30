#include <stdio.h>
#include <assert.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "imtk.h"

static int scr_width = 1, scr_height = 1;
static int mousex, mousey, mouse_bnmask;
static int active = -1, hot = -1;

static void set_active(int id);
static int set_hot(int id);
static int hit_test(int x, int y, int w, int h);

static void draw_button(int id, int x, int y, const char *label, int focused);
static void calc_button_size(const char *label, int *wret, int *hret);
static void draw_string(int x, int y, const char *str);
static int string_size(const char *str);

void imtk_inp_key(int key, int state)
{
	glutPostRedisplay();
}

void imtk_inp_mouse(int bn, int state)
{
	if(state == IMTK_DOWN) {
		mouse_bnmask |= 1 << bn;
	} else {
		mouse_bnmask &= ~(1 << bn);
	}
	glutPostRedisplay();
}

void imtk_inp_motion(int x, int y)
{
	mousex = x;
	mousey = y;

	glutPostRedisplay();
}

void imtk_inp_reshape(int x, int y)
{
	scr_width = x;
	scr_height = y;
}

void imtk_begin(void)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1, 1, 0);
	glScalef(2.0 / scr_width, -2.0 / scr_height, 1.0);
}

void imtk_end(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

int imtk_button(int id, const char *label, int x, int y)
{
	int w, h, res = 0;
	int over = 0;

	assert(id >= 0);

	calc_button_size(label, &w, &h);

	if(hit_test(x, y, w, h)) {
		if(set_hot(id)) {
			glutPostRedisplay();
		}
		over = 1;
	}


	if(mouse_bnmask & (1 << IMTK_LEFT_BUTTON)) {
		if(over) {
			set_active(id);
		}
	} else { /* mouse button up */
		if(active == id) {
			set_active(-1);
			if(hot == id && over) {
				res = 1;
			}
		}
	}

	draw_button(id, x, y, label, over);
	return res;
}

static void set_active(int id)
{
	active = id;
}

static int set_hot(int id)
{
	if(active == -1) {
		hot = id;
		return 1;
	}
	return 0;
}

static int hit_test(int x, int y, int w, int h)
{
	return mousex >= x && mousex < (x + w) &&
		mousey >= y && mousey < (y + h);
}

static void draw_button(int id, int x, int y, const char *label, int focused)
{
	int width, height;

	calc_button_size(label, &width, &height);

	glBegin(GL_QUADS);
	if(focused) {
		glColor3f(0.85, 0.85, 0.85);
	} else {
		glColor3f(0.7, 0.7, 0.7);
	}
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();

	glColor3f(0, 0, 0);
	draw_string(x + 20, y + 15, label);
}

static void calc_button_size(const char *label, int *wret, int *hret)
{
	int strsz = string_size(label);
	if(wret) *wret = strsz + 40;
	if(hret) *hret = 20;
}

static void draw_string(int x, int y, const char *str)
{
	glRasterPos2i(x, y);
	while(*str) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *str++);
	}
}

static int string_size(const char *str)
{
	return glutBitmapLength(GLUT_BITMAP_HELVETICA_10, str);
}
