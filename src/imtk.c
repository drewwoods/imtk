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
#include "state.h"


void imtk_begin(void)
{
	int width, height;

	imtk_get_viewport(&width, &height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1, 1, 0);
	glScalef(2.0 / width, -2.0 / height, 1.0);

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


/*int imtk_listbox(int id, const char *list, int sel, int x, int y)
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
}*/
