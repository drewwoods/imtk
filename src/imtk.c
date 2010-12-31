#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "imtk.h"

#define CHECKBOX_SIZE	14
#define TEXTBOX_SIZE	100
#define SLIDER_SIZE		100
#define THUMB_WIDTH		10
#define THUMB_HEIGHT	20

enum {
	FRAME_OUTSET,
	FRAME_INSET
};

struct key_node {
	int key;
	struct key_node *next;
};

static void set_active(int id);
static int set_hot(int id);
static int hit_test(int x, int y, int w, int h);

static void draw_button(int id, const char *label, int x, int y);
static void calc_button_size(const char *label, int *wret, int *hret);
static void draw_checkbox(int id, const char *label, int x, int y, int state);
static void draw_textbox(int id, const char *text, int x, int y);
static void draw_slider(int id, float pos, float min, float max, int x, int y);
static void draw_progress(int id, float pos, int x, int y);
static void draw_string(int x, int y, const char *str);
static int string_size(const char *str);
static void draw_frame(int x, int y, int w, int h, int style);


/* default colors, can be changed with imtk_set_color */
static float colors[][4] = {
	{0.0, 0.0, 0.0},	/* text color */
	{0.7, 0.7, 0.7},	/* base color */
	{0.85, 0.85, 0.85},	/* focus color */
	{1.0, 1.0, 1.0},	/* lit bevel */
	{0.3, 0.3, 0.3}		/* shadowed bevel */
};

static int scr_width = 1, scr_height = 1;
static int mousex, mousey, prevx, mouse_bnmask;
static int active = -1, hot = -1, input = -1;

static struct key_node *key_list, *key_tail;


void imtk_set_color(int col, float r, float g, float b)
{
	assert(col >= 0 && col < sizeof colors / sizeof *colors);

	colors[col][0] = r;
	colors[col][1] = g;
	colors[col][2] = b;
}

void imtk_inp_key(int key, int state)
{
	if(state == IMTK_DOWN) {
		struct key_node *node;

		if(!(node = malloc(sizeof *node))) {
			return;
		}
		node->key = key;
		node->next = 0;

		if(key_list) {
			key_tail->next = node;
			key_tail = node;
		} else {
			key_list = key_tail = node;
		}
	}

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

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
}

void imtk_end(void)
{
	glPopAttrib();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
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

void imtk_textbox(int id, char *textbuf, size_t buf_sz, int x, int y)
{
	int len, over = 0;

	assert(id >= 0);

	if(hit_test(x, y, TEXTBOX_SIZE, 20)) {
		set_hot(id);
		over = 1;
	}

	if(mouse_bnmask & (1 << IMTK_LEFT_BUTTON)) {
		if(over) {
			set_active(id);
		}
	} else {
		if(active == id) {
			set_active(-1);
			if(hot == id && over) {
				input = id;
			}
		}
	}

	if(input == id) {
		len = strlen(textbuf);
		while(key_list) {
			struct key_node *node = key_list;
			key_list = key_list->next;

			if(isprint(node->key)) {
				if(len < buf_sz) {
					textbuf[len++] = (char)node->key;
				}
			} else {
				switch(node->key) {
				case '\b':
					if(len > 0) {
						textbuf[--len] = 0;
					}
					break;

				default:
					break;
				}
			}

			free(node);
		}
		key_list = key_tail = 0;
	}

	draw_textbox(id, textbuf, x, y);
}

float imtk_slider(int id, float pos, float min, float max, int x, int y)
{
	int thumb_x, thumb_y, over = 0;
	float range = max - min;

	assert(id >= 0);

	pos = (pos - min) / range;
	if(pos < 0.0) pos = 0.0;
	if(pos > 1.0) pos = 1.0;

	thumb_x = x + SLIDER_SIZE * pos - THUMB_WIDTH / 2;
	thumb_y = y - THUMB_HEIGHT / 2;

	if(hit_test(thumb_x, thumb_y, THUMB_WIDTH, THUMB_HEIGHT)) {
		set_hot(id);
		over = 1;
	}

	if(mouse_bnmask & (1 << IMTK_LEFT_BUTTON)) {
		if(over && hot == id) {
			if(active != id) {
				prevx = mousex;
			}
			set_active(id);
		}
	} else {
		if(active == id) {
			set_active(-1);
		}
	}

	if(active == id) {
		float dx = (float)(mousex - prevx) / (float)SLIDER_SIZE;
		pos += dx;
		prevx = mousex;

		if(pos < 0.0) pos = 0.0;
		if(pos > 1.0) pos = 1.0;
	}

	draw_slider(id, pos, min, max, x, y);
	return pos * range + min;
}

void imtk_progress(int id, float pos, int x, int y)
{
	draw_progress(id, pos, x, y);
}

static void set_active(int id)
{
	if(id == -1 || hot == id) {
		active = id;
	}
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

static void draw_textbox(int id, const char *text, int x, int y)
{
	int strsz = string_size(text);

	if(hit_test(x, y, TEXTBOX_SIZE, 20)) {
		glColor3fv(colors[IMTK_FOCUS_COLOR]);
	} else {
		glColor3fv(colors[IMTK_BASE_COLOR]);
	}

	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + TEXTBOX_SIZE, y);
	glVertex2f(x + TEXTBOX_SIZE, y + 20);
	glVertex2f(x, y + 20);
	glEnd();

	glColor3fv(colors[IMTK_TEXT_COLOR]);

	if(input == id) {
		glBegin(GL_LINES);
		glVertex2f(x + strsz + 2, y + 2);
		glVertex2f(x + strsz + 2, y + 18);
		glEnd();
	}

	draw_string(x + 2, y + 15, text);

	draw_frame(x, y, TEXTBOX_SIZE, 20, FRAME_INSET);
}

static void draw_slider(int id, float pos, float min, float max, int x, int y)
{
	float range = max - min;
	int thumb_x, thumb_y;
	char buf[32];

	thumb_x = x + SLIDER_SIZE * pos - THUMB_WIDTH / 2;
	thumb_y = y - THUMB_HEIGHT / 2;

	/* draw trough */
	glBegin(GL_QUADS);
	glColor3fv(colors[IMTK_BASE_COLOR]);
	glVertex2f(x, y - 2);
	glVertex2f(x + SLIDER_SIZE, y - 2);
	glVertex2f(x + SLIDER_SIZE, y + 2);
	glVertex2f(x, y + 2);
	glEnd();
	draw_frame(x, y - 2, SLIDER_SIZE, 4, FRAME_INSET);

	if(hit_test(thumb_x, thumb_y, THUMB_WIDTH, THUMB_HEIGHT)) {
		glColor3fv(colors[IMTK_FOCUS_COLOR]);
	} else {
		glColor3fv(colors[IMTK_BASE_COLOR]);
	}

	/* draw handle */
	glBegin(GL_QUADS);
	glVertex2f(thumb_x, thumb_y);
	glVertex2f(thumb_x + THUMB_WIDTH, thumb_y);
	glVertex2f(thumb_x + THUMB_WIDTH, thumb_y + THUMB_HEIGHT);
	glVertex2f(thumb_x, thumb_y + THUMB_HEIGHT);
	glEnd();
	draw_frame(thumb_x, thumb_y, THUMB_WIDTH, THUMB_HEIGHT, FRAME_OUTSET);

	/* draw display */
	sprintf(buf, "%.3f", pos * range + min);
	glColor3fv(colors[IMTK_TEXT_COLOR]);
	draw_string(x + SLIDER_SIZE + THUMB_WIDTH / 2 + 2, y + 4, buf);
}

static void draw_progress(int id, float pos, int x, int y)
{
	int bar_size = SLIDER_SIZE * pos;

	if(pos < 0.0) pos = 0.0;
	if(pos > 1.0) pos = 1.0;

	/* through */
	glBegin(GL_QUADS);
	glColor3fv(colors[IMTK_BASE_COLOR]);
	glVertex2f(x - 1, y - 1);
	glVertex2f(x + SLIDER_SIZE + 1, y - 1);
	glVertex2f(x + SLIDER_SIZE + 1, y + 17);
	glVertex2f(x - 1, y + 17);
	glEnd();
	draw_frame(x - 1, y - 1, SLIDER_SIZE + 2, 17, FRAME_INSET);

	if(pos > 0.0) {
		/* bar */
		glBegin(GL_QUADS);
		glColor3fv(colors[IMTK_BASE_COLOR]);
		glVertex2f(x, y);
		glVertex2f(x + bar_size, y);
		glVertex2f(x + bar_size, y + 15);
		glVertex2f(x, y + 15);
		glEnd();
		draw_frame(x, y, bar_size, 15, FRAME_OUTSET);
	}
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
