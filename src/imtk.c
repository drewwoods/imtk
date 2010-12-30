#include <stdio.h>
#include <string.h>
#include <assert.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "imtk.h"

#define CHECKBOX_SIZE	14

enum {
	FRAME_OUTSET,
	FRAME_INSET
};

/* default colors, can be changed with imtk_set_color */
static float colors[][4] = {
	{0.0, 0.0, 0.0},	/* text color */
	{0.7, 0.7, 0.7},	/* base color */
	{0.85, 0.85, 0.85},	/* focus color */
	{1.0, 1.0, 1.0},	/* lit bevel */
	{0.3, 0.3, 0.3}		/* shadowed bevel */
};

static int scr_width = 1, scr_height = 1;
static int mousex, mousey, mouse_bnmask;
static int active = -1, hot = -1;

static void set_active(int id);
static int set_hot(int id);
static int hit_test(int x, int y, int w, int h);

static void draw_button(int id, const char *label, int x, int y);
static void calc_button_size(const char *label, int *wret, int *hret);
static void draw_checkbox(int id, const char *label, int x, int y, int state);
static void draw_string(int x, int y, const char *str);
static int string_size(const char *str);
static void draw_frame(int x, int y, int w, int h, int style);

void imtk_set_color(int col, float r, float g, float b)
{
	assert(col >= 0 && col < sizeof colors / sizeof *colors);

	colors[col][0] = r;
	colors[col][1] = g;
	colors[col][2] = b;
}

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
		set_hot(id);
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

	draw_button(id, label, x, y);
	return res;
}

int imtk_checkbox(int id, const char *label, int x, int y, int state)
{
	int sz = CHECKBOX_SIZE;
	int over = 0;

	assert(id >= 0);

	if(hit_test(x, y, sz, sz)) {
		set_hot(id);
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
				state = !state;
			}
		}
	}

	draw_checkbox(id, label, x, y, state);
	return state;
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

static void draw_button(int id, const char *label, int x, int y)
{
	int width, height;

	calc_button_size(label, &width, &height);

	glBegin(GL_QUADS);
	if(hit_test(x, y, width, height)) {
		glColor3fv(colors[IMTK_FOCUS_COLOR]);
	} else {
		glColor3fv(colors[IMTK_BASE_COLOR]);
	}
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();

	draw_frame(x, y, width, height, active == id ? FRAME_INSET : FRAME_OUTSET);

	glColor3fv(colors[IMTK_TEXT_COLOR]);
	draw_string(x + 20, y + 15, label);
}

static void calc_button_size(const char *label, int *wret, int *hret)
{
	int strsz = string_size(label);
	if(wret) *wret = strsz + 40;
	if(hret) *hret = 20;
}

static void draw_checkbox(int id, const char *label, int x, int y, int state)
{
	static const int sz = CHECKBOX_SIZE;

	if(hit_test(x, y, sz, sz)) {
		glColor3fv(colors[IMTK_FOCUS_COLOR]);
	} else {
		glColor3fv(colors[IMTK_BASE_COLOR]);
	}

	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + sz, y);
	glVertex2f(x + sz, y + sz);
	glVertex2f(x, y + sz);
	glEnd();

	draw_frame(x, y, sz, sz, FRAME_INSET);

	glColor3fv(colors[IMTK_TEXT_COLOR]);
	if(state) {
		glPushAttrib(GL_LINE_BIT);
		glLineWidth(2);

		glBegin(GL_LINES);
		glVertex2f(x + 2, y + 2);
		glVertex2f(x + sz - 2, y + sz - 2);
		glVertex2f(x + sz - 2, y + 2);
		glVertex2f(x + 2, y + sz - 2);
		glEnd();

		glPopAttrib();
	}

	draw_string(x + sz + 5, y + sz - 2, label);
}

static void draw_string(int x, int y, const char *str)
{
	glRasterPos2i(x, y);
	while(*str) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str++);
	}
}

static int string_size(const char *str)
{
	return glutBitmapLength(GLUT_BITMAP_HELVETICA_12, (const unsigned char*)str);
}

static void draw_frame(int x, int y, int w, int h, int style)
{
	float tcol[3], bcol[3];

	switch(style) {
	case FRAME_INSET:
		memcpy(tcol, colors[IMTK_BEVEL_SHAD_COLOR], sizeof tcol);
		memcpy(bcol, colors[IMTK_BEVEL_LIT_COLOR], sizeof bcol);
		break;

	case FRAME_OUTSET:
	default:
		memcpy(tcol, colors[IMTK_BEVEL_LIT_COLOR], sizeof tcol);
		memcpy(bcol, colors[IMTK_BEVEL_SHAD_COLOR], sizeof bcol);
	}

	glBegin(GL_LINES);
	glColor3fv(tcol);
	glVertex2f(x, y + h);
	glVertex2f(x, y);
	glVertex2f(x, y);
	glVertex2f(x + w, y);
	glColor3fv(bcol);
	glVertex2f(x + w, y);
	glVertex2f(x + w, y + h);
	glVertex2f(x + w, y + h);
	glVertex2f(x, y + h);
	glEnd();
}
