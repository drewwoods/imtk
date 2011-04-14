#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <assert.h>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "imtk.h"


static void draw_progress(int id, float pos, int x, int y);


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


void imtk_post_redisplay(void)
{
	glutPostRedisplay();
}



void imtk_progress(int id, float pos, int x, int y)
{
	draw_progress(id, pos, x, y);
}

int imtk_listbox(int id, const char *list, int sel, int x, int y)
{
	int i;
	assert(id >= 0);

	if(!list) {
		return -1;
	}

	if(id & 1) {
		id++;
	}

	for(i=0; *list; i++) {
		if(imtk_button(id + i * 2 + 1, list, x, y + i * 20)) {
			sel = i;
		}
		list += strlen(list) + 1;
	}
	return sel;
}

int imtk_combobox(int id, char *textbuf, size_t buf_sz, const char *list, int sel, int x, int y)
{
	imtk_textbox(id + 1, textbuf, buf_sz, x, y);
	imtk_button(id + 3, "V", x + TEXTBOX_SIZE, y);

	if(prev_active == id + 3) {
		sel = imtk_listbox(id + 5, list, sel, x, y + 20);
	}
	return sel;
}

char *imtk_create_list(const char *first, ...)
{
	int sz;
	char *buf, *item;
	va_list ap;

	if(!first) {
		return 0;
	}

	sz = strlen(first) + 2;
	if(!(buf = malloc(sz))) {
		return 0;
	}
	memcpy(buf, first, sz - 2);
	buf[sz - 1] = buf[sz - 2] = 0;

	va_start(ap, first);
	while((item = va_arg(ap, char*))) {
		int len = strlen(item);
		char *tmp = realloc(buf, sz + len + 1);
		if(!tmp) {
			free(buf);
			return 0;
		}
		buf = tmp;

		memcpy(buf + sz - 1, item, len);
		sz += len + 1;
		buf[sz - 1] = buf[sz - 2] = 0;
	}
	va_end(ap);

	return buf;
}

void imtk_free_list(char *list)
{
	free(list);
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
