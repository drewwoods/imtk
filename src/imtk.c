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
#include "state.h"
#include "draw.h"


void imtk_post_redisplay(void)
{
	glutPostRedisplay();
}

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
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void imtk_end(void)
{
	glPopAttrib();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void imtk_label(const char *str, int x, int y)
{
	glColor4fv(imtk_get_color(IMTK_TEXT_COLOR));
	imtk_draw_string(x, y, str);
}


/*
int imtk_combobox(int id, char *textbuf, size_t buf_sz, const char *list, int sel, int x, int y)
{
	imtk_textbox(id + 1, textbuf, buf_sz, x, y);
	imtk_button(id + 3, "V", x + TEXTBOX_SIZE, y);

	if(prev_active == id + 3) {
		sel = imtk_listbox(id + 5, list, sel, x, y + 20);
	}
	return sel;
}
*/
